#pragma once
#include "vpch.h"
#include "glm/mat4x4.hpp"

namespace Vectora {
	class VECTORA_API Camera
	{
	public:
		virtual ~Camera() = default;
		virtual const glm::mat4& GetProjection() const = 0;
		virtual const glm::mat4& GetPV() const = 0;
		virtual void SetRotation(float rotation) = 0;
		virtual float GetRotation() const = 0;
		virtual void SetPosition(glm::vec3 dir) = 0;
		virtual const glm::vec3& GetPosition() const = 0;
	};

}