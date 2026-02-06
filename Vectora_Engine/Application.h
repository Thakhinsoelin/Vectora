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
#include <math.h>

namespace Vectora {

	struct BlacHole {
		double c = 299792458.0;
		double G = 6.67430e-11;
		glm::vec3 position;
		double mass;
		double radius;
		double r_s;
		int holeSeg;
		BlacHole() {

		}
		BlacHole(glm::vec3 pos, double mass)
			:position(pos), mass(mass) {
			r_s = 2.0 * G * mass / (c * c);
			radius = 0.2f;
			m_HoleVA.reset(VertexArray::Create());
			int segments = 100;
			holeSeg = segments + 2;
			circlePoints.push_back(pos.x);
			circlePoints.push_back(pos.y);
			for (size_t i = 0; i <= segments; i++)
			{
				float angle = 2.0f * 3.14159265358979323846264338327950288 * i / segments;
				circlePoints.push_back(radius * cos(angle));
				circlePoints.push_back(radius * sin(angle));
			}

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(circlePoints.data(), circlePoints.size()* sizeof(float)));
			BufferLayout layout = {
				{ ShaderDataType::Float2, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);
			m_HoleVA->AddVertexBuffer(vertexBuffer);

			uint32_t indices[3] = { 0, 1, 2 };
			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_HoleVA->SetIndexBuffer(indexBuffer);

			m_HoleShader = std::make_shared<Shader>("shaders/circleVt.glsl", "shaders/circleFg.glsl");
			m_HoleShader->createShaders(BOTH_FROM_FILE);
			m_HoleShader->Bind();

		};
		void setStuff(glm::vec3 pos, double mass) {
			this->position = pos;
			this->mass = mass;
			this->r_s = 2.0 * G * mass / (c * c);
		}

		void draw() {
			m_HoleShader->Bind();
			m_HoleShader->setVec2("offset", 0.0, 0.0f);
			m_HoleVA->Bind();
			glDrawArrays(GL_TRIANGLE_FAN, 0, holeSeg);
		}

	private:
		std::vector<float> circlePoints;
		std::shared_ptr<VertexArray> m_HoleVA;
		std::shared_ptr<Shader> m_HoleShader;
	};

	struct Ray {
		glm::vec2 origin;
		glm::vec2 direction;
		double r, phi;
		Ray(glm::vec2 ori, glm::vec2 dir)
			:origin(ori), direction(dir) {
			r = sqrt(origin.x * origin.x + origin.y * origin.y);
			phi = atan2(ori.y, ori.x);
			m_RayVA.reset(VertexArray::Create());
			float vertices[] = {
				origin.x, origin.y,
				origin.x + direction.x, origin.y + direction.y
			};

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
			m_RayShader->setVec2("mov", this->origin.x, this->origin.y);
			m_RayVA->Bind();
			glDrawArrays(GL_LINES, 0,2 );
		}
		void step(double r_s) {
			r = sqrt(origin.x * origin.x + origin.y * origin.y);
			if (r < r_s) {
				return;
			}
			origin += direction * 0.025f;

			// 4. NEW: Check if we just entered the event horizon
			double new_r = sqrt(origin.x * origin.x + origin.y * origin.y);
			if (new_r < r_s) {
				// Optional: Snap the ray exactly to the edge of r_s for a clean look
				glm::vec2 unit_pos = glm::normalize(origin);
				origin = unit_pos * (float)r_s;
			}
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

		
		std::vector<Ray> rays;
		BlacHole SagA;
	};
	Application* CreateApplication();
}

