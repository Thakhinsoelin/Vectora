#include "Renderer/Shader.h"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace Vectora {
	void Shader::getShaderCompilationError(unsigned int shaderID, GLenum ERROR_TYPE, ShaderType type) {
		int success;
		char log[512];
		std::string stype;
		if (type == VERTEX) {
			stype = "VERTEX";
		}
		else {
			stype = "FRAGMENT";
		}
		glGetShaderiv(shaderID, ERROR_TYPE, &success);

		if (!success) {
			glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
			printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", stype.c_str(), log);
		}
	}

	void Shader::getShaderLinkError(unsigned int program, GLenum ERROR_TYPE) {
		int success;
		char log[512];
		glGetProgramiv(program, ERROR_TYPE, &success);

		if (!success) {
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
			printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", log);
		}
	}

	Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
	{
		loadShaders(vertexShaderPath, fragmentShaderPath);
	};

	Shader::Shader(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
		this->vertexShader = vertexShaderID;
		this->fragmentShader = fragmentShaderID;
	}

	Shader::Shader(std::string vertexShaderPath, unsigned int fragmentShaderID) {
		loadVertexShader(vertexShaderPath);
		this->fragmentShader = fragmentShaderID;
	}

	Shader::Shader(unsigned int vertexShaderID, std::string fragmentShaderPath) {
		this->vertexShader = vertexShaderID;
		loadFragmentShader(fragmentShaderPath);
	}

	void Shader::Bind()
	{
		glUseProgram(RenderID);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	void Shader::loadShaders(std::string vPath, std::string fPath) {
		loadVertexShader(vPath);
		loadFragmentShader(fPath);
	}

	void Shader::loadVertexShader(std::string vPath) {
		vertexShaderSource.clear();
		std::ifstream vShaderFile(vPath);
		std::string line;
		if (vShaderFile) {
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vertexShaderSource = vShaderStream.str();
			vShaderFile.close();
		}
		else {
			printf("Failed to open: %s\n", vPath.c_str());
		}
	}

	void Shader::loadFragmentShader(std::string fPath) {
		fragmentShaderSource.clear();
		std::ifstream fShaderFile(fPath);
		std::string line;

		if (fShaderFile) {
			std::stringstream vShaderStream;
			vShaderStream << fShaderFile.rdbuf();
			fragmentShaderSource = vShaderStream.str();
			fShaderFile.close();
		}
		else {
			printf("Failed to open fragment shader file at path: %s\n", fPath.c_str());
		}

	}

	void Shader::createShaders(ShaderCreationMode mode) {

		if (mode == ShaderCreationMode::BOTH_FROM_FILE)
		{
			compileVertexShader();
			compileFragmentShader();
			linkShaders();
		}
		else if (mode == ShaderCreationMode::BOTH_FROM_ID) {
			linkShaders();
		}
		else if (mode == ShaderCreationMode::VERTEX_ID_FRAG_FILE) {
			compileFragmentShader();
			linkShaders();
		}
		else if (mode == ShaderCreationMode::VERTEX_FILE_FRAG_ID) {
			compileVertexShader();
			linkShaders();
		}

	};

	void Shader::useProgram() {
		glUseProgram(RenderID);
	};

	void Shader::compileVertexShader() {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSource = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		getShaderCompilationError(vertexShader, GL_COMPILE_STATUS, VERTEX);
	}

	void Shader::compileFragmentShader() {
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragSource = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);
		getShaderCompilationError(fragmentShader, GL_COMPILE_STATUS, FRAGMENT);
	}

	void Shader::linkShaders() {
		RenderID = glCreateProgram();
		glAttachShader(RenderID, vertexShader);
		glAttachShader(RenderID, fragmentShader);
		glLinkProgram(RenderID);
		getShaderLinkError(RenderID, GL_LINK_STATUS);
	}

	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(RenderID, name.c_str()), value);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(RenderID, name.c_str()), value);
	}
	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(RenderID, name.c_str()), value);
	}

	void Shader::setVec2(const std::string& name, float v1, float v2) const {
		glUniform2f(glGetUniformLocation(RenderID, name.c_str()), v1, v2);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4& vec4) const
	{
		glUniform4f(glGetUniformLocation(RenderID, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
	}
	;

	void Shader::setMat4(const std::string& name, const glm::mat4& trans) const {
		glUniformMatrix4fv(glGetUniformLocation(RenderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
	}

}