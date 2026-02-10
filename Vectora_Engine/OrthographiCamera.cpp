#include "OrthographiCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Vectora {
	OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{

	}
	OrthoGraphicCamera::~OrthoGraphicCamera()
	{
	}
	const glm::mat4& OrthoGraphicCamera::GetProjection() const
	{
		// TODO: insert return statement here
		return m_ProjectionMatrix;
	}
	const glm::mat4& OrthoGraphicCamera::GetPV() const
	{
		// TODO: insert return statement here
		return m_ViewProjectionMatrix;
	}
	void OrthoGraphicCamera::SetRotation(float rotation)
	{
		this->m_Rotation = rotation;
		RecalculateViewMatrix();
		1 + 2;
	}
	float OrthoGraphicCamera::GetRotation(void) const
	{
		return this->m_Rotation;
	}
	
	void OrthoGraphicCamera::SetPosition(glm::vec3 dir)
	{
		this->m_Position = dir;
		RecalculateViewMatrix();
	}

	const glm::vec3& OrthoGraphicCamera::GetPosition() const
	{
		return this->m_Position;
	}
	void OrthoGraphicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
