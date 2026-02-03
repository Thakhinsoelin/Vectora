#include "shaderLoader.h"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

void shaderLoader::getShaderCompilationError(unsigned int shaderID, GLenum ERROR_TYPE, ShaderType type) {
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

void shaderLoader::getShaderLinkError(unsigned int program, GLenum ERROR_TYPE) {
	int success;
	char log[512];
	glGetProgramiv(program, ERROR_TYPE, &success);

	if (!success) {
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", log);
	}
}

shaderLoader::shaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	loadShaders(vertexShaderPath, fragmentShaderPath);
};

shaderLoader::shaderLoader(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
	this->vertexShader = vertexShaderID;
	this->fragmentShader = fragmentShaderID;
}

shaderLoader::shaderLoader(std::string vertexShaderPath, unsigned int fragmentShaderID) {
	loadVertexShader(vertexShaderPath);
	this->fragmentShader = fragmentShaderID;
}

shaderLoader::shaderLoader(unsigned int vertexShaderID, std::string fragmentShaderPath) {
	this->vertexShader = vertexShaderID;
	loadFragmentShader(fragmentShaderPath);
}

void shaderLoader::loadShaders(std::string vPath, std::string fPath) {
	loadVertexShader(vPath);
	loadFragmentShader(fPath);
}

void shaderLoader::loadVertexShader(std::string vPath) {
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

void shaderLoader::loadFragmentShader(std::string fPath) {
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

void shaderLoader::createShaders(ShaderCreationMode mode) {
	
	if(mode == ShaderCreationMode::BOTH_FROM_FILE)
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

void shaderLoader::useProgram() {
	glUseProgram(shaderProgram);
};

void shaderLoader::compileVertexShader() {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	getShaderCompilationError(vertexShader, GL_COMPILE_STATUS, VERTEX);
}

void shaderLoader::compileFragmentShader() {
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragSource = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	getShaderCompilationError(fragmentShader, GL_COMPILE_STATUS, FRAGMENT);
}

void shaderLoader::linkShaders() {
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	getShaderLinkError(shaderProgram, GL_LINK_STATUS);
}

void shaderLoader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void shaderLoader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void shaderLoader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void shaderLoader::setVec2(const std::string& name, float v1, float v2) const {
	glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2);
};

void shaderLoader::setMat4(const std::string& name, glm::mat4& trans) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}