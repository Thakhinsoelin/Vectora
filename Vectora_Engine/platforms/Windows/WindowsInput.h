#pragma once
#include "Input.h"

namespace Vectora {
    class WindowsInput : public Input
    {
	protected:
		virtual bool IsKeyPressedImpl(VE_KEYCODE keycode) override final;

		virtual bool IsMouseButtonPressedImpl(VE_KEYCODE button) override final;
		virtual MousePos GetMousePositionImpl() override final;
		virtual float GetMouseXImpl() override final;
		virtual float GetMouseYImpl() override final;
    };
}

