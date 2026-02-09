#pragma once
#include "vpch.h"
#include "glm/mat4x4.hpp"

namespace Vectora {
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		virtual ~Camera() = default;
		const glm::mat4& GetProjection() const { return m_Projection; }
	private:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix;
	};

}