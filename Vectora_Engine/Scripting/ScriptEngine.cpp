#include "vpch.h"
#include "ScriptEngine.h"
#include "Vectora.h"


#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Vectora {

	struct ScriptEngineData {
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data;

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			printf("%s.%s\n", nameSpace, name);
		}
	}


	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();

	}
	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	static void NativeLog(MonoString* text, int param) {
		char* cStr = mono_string_to_utf8(text);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << param << "\n";
	}

	static void NativeLogV3(glm::vec3* parameter) {
		//VE_CORE_WARN("Value: {0}", *parameter);
		glm::vec3 ran(1.f, 2.f, 3.f);
		VE_WARN("{0}", ran);
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono");

		MonoDomain* rootDomain = mono_jit_init("VectoraJITRuntime");
		VE_CORE_ASSERT(rootDomain);

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;
		std::string tt = "VectoraAppDomain";
		s_Data->AppDomain = mono_domain_create_appdomain(const_cast<char*>("VectoraScriptRuntime"), nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		mono_add_internal_call("Vectora.Main::NativeLog", NativeLog);
		mono_add_internal_call("Vectora.Main::NativeLog_Vector", NativeLogV3);

		s_Data->CoreAssembly = LoadCSharpAssembly("Vectora-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "Vectora", "Main");

		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);


		int value = 5;
		void* param = &value;
		void* args[] = { &value };
		MonoMethod* printMessageInt = mono_class_get_method_from_name(monoClass, "PrintCustomInt", 1);
		mono_runtime_invoke(printMessageInt, instance, args, nullptr);

		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello from C++");
		void* stringArgs[] = { monoString };
		MonoMethod* printCustomMessage = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		mono_runtime_invoke(printCustomMessage, instance, stringArgs, nullptr);
	}
	void ScriptEngine::ShutdownMono()
	{
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		//mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}
}