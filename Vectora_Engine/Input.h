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
		inline static bool IsKeyPressed(VE_KEYCODE keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(VE_KEYCODE button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static MousePos GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(VE_KEYCODE keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(VE_KEYCODE button) = 0;
		virtual MousePos GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}