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
		bool active = true;
		Ray(glm::vec2 ori, glm::vec2 dir)
			:origin(ori), direction(dir ) {
			r = sqrt(origin.x * origin.x + origin.y * origin.y);
			phi = atan2(ori.y, ori.x);
			m_RayVA.reset(VertexArray::Create());
			float vertices[] = {
				0, 0,
				-direction.x, -direction.y
			};

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			BufferLayout layout = {
				{ ShaderDataType::Float2, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);
			m_RayVA->AddVertexBuffer(vertexBuffer);

			

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
			if (!active) return;
			r = sqrt(origin.x * origin.x + origin.y * origin.y);
			if (r < r_s) {
				return;
			}
			// let's say dir = 0.5, 0 for example
			origin.x += direction.x * 0.0025f;

			// 4. NEW: Check if we just entered the event horizon
			//double new_r = sqrt(origin.x * origin.x + origin.y * origin.y);
			//if (new_r < r_s) {
			//	// Optional: Snap the ray exactly to the edge of r_s for a clean look
			//	glm::vec2 unit_pos = glm::normalize(origin);
			//	origin = unit_pos * (float)r_s;
			//}
		}

		void gpt_step(double r_s, glm::vec2 bh_pos) {
			if (!active) return;
			glm::vec2 next_origin = (origin + direction * 0.0025f) ;

				float dist_now = glm::distance(origin, bh_pos);
				float dist_next = glm::distance(next_origin, bh_pos);
				glm::vec2 relativePos = origin - bh_pos;
				float r_dist = glm::length(relativePos);
				// If it WAS outside but WILL BE inside
				if (r_dist <= r_s) {
					active = false;
					//// 1. Calculate the exact intersection point (Optional "Snap")
					//glm::vec2 to_ray = glm::normalize(next_origin - bh_pos);
					//origin = bh_pos + to_ray * (float)r_s;
					// here
					//origin.x = bh_pos.x + glm::normalize(next_origin - bh_pos).x * (float)r_s;
					origin.x = bh_pos.x + glm::normalize(relativePos).x * (float)r_s;
					//// 2. Kill the ray
					
					return;
				}
				// 3. GRAVITY: Pull the direction vector toward the center
	// Math: Force is proportional to 1/r^2 (simplified for visual effect)
				glm::vec2 pull_direction = glm::normalize(bh_pos - origin);

				// We use a small 'strength' factor. 
				// In real physics, this involves the mass, but for your 800x800 screen:
				float gravity_strength = 0.0005f / (r_dist * r_dist);

				direction += pull_direction * gravity_strength;

				// IMPORTANT: Light speed is constant, so we re-normalize direction
				direction = glm::normalize(direction);

				// 4. Move
				origin += direction * 0.005f; // Adjust speed here
				//origin = next_origin;
			
		}

		//unsigned int VAO, VBO, VEO;;
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

