#pragma once
#include "Core/Camera.h"

namespace Vectora {
	class VECTORA_API OrthoGraphicCamera : public Camera
	{
	public:

		OrthoGraphicCamera(float left, float right, float bottom, float top);
		
		void SetProjection(float left, float right, float bottom, float top);

		~OrthoGraphicCamera();
		virtual const glm::mat4& GetProjection() const override;
		virtual const glm::mat4& GetPV() const override;
		virtual void SetRotation(float rotation) override final;
		virtual float GetRotation(void) const override final;
		void SetPosition(glm::vec3 dir);
		virtual const glm::vec3& GetPosition() const override final;


	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}