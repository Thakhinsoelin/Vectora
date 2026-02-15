#pragma once
#include "Renderer/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>

namespace Vectora {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string vertexShaderPath, std::string fragmentShaderPath);
		OpenGLShader(unsigned int vertexShaderID, unsigned int fragmentShaderID);
		OpenGLShader(std::string vertexShaderPath, unsigned int fragmentShaderID);
		OpenGLShader(unsigned int vertexShaderID, std::string fragmentShaderPath);
		~OpenGLShader() {
			glDeleteProgram(RenderID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader); 
			// You can also delete the shaders themselves after linking
		}

		virtual void Bind() const override final;
		virtual void UnBind() const override final;
		virtual void createShaders(ShaderCreationMode mode) override final;
		virtual void useProgram() const override final;

		std::string getVertexShaderSource() {
			return vertexShaderSource;
		}

		std::string getFragmentShaderSource() {
			return fragmentShaderSource;
		}

		unsigned int getVertexShaderID() {
			return vertexShader;
		}

		unsigned int getFragmentShaderID() {
			return fragmentShader;
		}

		unsigned int getProgramID() {
			return RenderID;
		}

		int getUniformLocation(const char* varName) {
			return glGetUniformLocation(RenderID, varName);
		}

		
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, float v1, float v2) const;
		void setVec3(const std::string& name, const glm::vec3& vec4) const;
		void setVec4(const std::string& name, const glm::vec4& vec4) const;
		void setMat4(const std::string& name, const glm::mat4& trans) const;

	private:
		void getShaderCompilationError(unsigned int shaderID, GLenum ERROR_TYPE, ShaderType type);
		void getShaderLinkError(unsigned int program, GLenum ERROR_TYPE);

		void compileVertexShader();
		void compileFragmentShader();
		void linkShaders();

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int RenderID;

		void loadShaders(std::string vPath, std::string fPath);
		void loadVertexShader(std::string vPath);
		void loadFragmentShader(std::string fPath);

		std::string vertexShaderSource;
		std::string fragmentShaderSource;
	};
}

