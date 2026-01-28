#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Vectora {
	class VECTORA_API Log
	{
	public:
		Log() {

		}
		~Log() {

		}
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define VE_CORE_ERROR(...)	::Vectora::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VE_CORE_WARN(...)	::Vectora::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VE_CORE_INFO(...)	::Vectora::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VE_CORE_TRACE(...)	::Vectora::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VE_CORE_FATAL(...)	::Vectora::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define VE_ERROR(...)	::Vectora::Log::GetClientLogger()->error(__VA_ARGS__)
#define VE_WARN(...)		::Vectora::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VE_INFO(...)		::Vectora::Log::GetClientLogger()->info(__VA_ARGS__)
#define VE_TRACE(...)	::Vectora::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VE_FATAL(...)	::Vectora::Log::GetClientLogger()->fatal(__VA_ARGS__)