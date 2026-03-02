#pragma once

#include "vpch.h"
#include "Core.h"
#include "KeyCodes.h"


namespace Vectora {

	struct MousePos {
		float xPos;
		float yPos;
		MousePos(float x, float y)
			: xPos(x), yPos(y) {
		}
	};

	class VECTORA_API Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool IsKeyPressed(VE_KEYCODE keycode) ;
		static bool IsMouseButtonPressed(VE_KEYCODE button) ;
		static MousePos GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

}