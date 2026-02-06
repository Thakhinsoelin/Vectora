#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "imgui/ImGuiLayer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

namespace Vectora {

	struct Ray {
		glm::vec2 origin;
		glm::vec2 direction;

		Ray(glm::vec2 ori, glm::vec2 dir)
			:origin(ori), direction(dir) {
			
			m_RayVA.reset(VertexArray::Create());
			float vertices[] = {
				origin.x, origin.y,
				origin.x + direction.x, origin.y + direction.y
			};
			/*float vertices[] = {
				-0.8f, 0.0f,
				 0.8f, 0.0f
						};
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);*/


			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			BufferLayout layout = {
				{ ShaderDataType::Float2, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);
			m_RayVA->AddVertexBuffer(vertexBuffer);

			uint32_t indices[3] = { 0, 1, 2 };
			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_RayVA->SetIndexBuffer(indexBuffer);

			m_RayShader = std::make_shared<Shader>("shaders/rayVt.glsl", "shaders/rayFg.glsl");
			m_RayShader->createShaders(BOTH_FROM_FILE);
			m_RayShader->Bind();
		}

		void draw() {
			m_RayShader->Bind();
			m_RayVA->Bind();
			glDrawArrays(GL_LINES, 0,2 );
		}
		void step() {
			origin += direction * 0.1f;
		}

		//unsigned int VAO, VBO, VEO;
	private:
		std::shared_ptr<VertexArray> m_RayVA;
		std::shared_ptr<Shader> m_RayShader;
	};

	class VECTORA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		bool OnWindowClosed(WindowCloseEvent& e);

		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window> window;
		
		ImGuiLayer* m_ImguiLayer;
		LayerStack layerstack;
		bool m_Running = true;
		static Application* s_Instance;
		

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;

		std::shared_ptr<VertexArray> m_CircleVA;
		std::shared_ptr<Shader> m_CircleShader;

		std::vector<Ray> rays;
	};
	Application* CreateApplication();
}

