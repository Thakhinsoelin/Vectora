#pragma once
#include "Events/Event.h"
#include "KeyCodes.h"

namespace Vectora {
	class VECTORA_API KeyEvent : public Event {
	public:
		inline VE_KEYCODE GetKeyCode() const { return m_KeyCode; }
		
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(VE_KEYCODE keycode)
			: m_KeyCode(keycode) {
		}
		VE_KEYCODE m_KeyCode;
	};

	class VECTORA_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(VE_KEYCODE keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {
		}
		bool IsRepeat() {
			return m_IsRepeat;
		}
		std::string ToString() const override {
			return std::string("KeyPressedEvent: ") + std::to_string(m_KeyCode) + " (repeat = " + std::to_string(m_IsRepeat) + ")";
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		
		bool m_IsRepeat;
	};

	class VECTORA_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(VE_KEYCODE keycode)
			: KeyEvent(keycode) {
		}
		std::string ToString() const override {
			return std::string("KeyReleasedEvent: ") + std::to_string(m_KeyCode);
		}
		EVENT_CLASS_TYPE(KeyReleased)
	
	};

	class VECTORA_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(VE_KEYCODE keycode)
			: KeyEvent(keycode) {
		}
		std::string ToString() const override {
			return std::string("KeyTypedEvent: ") + std::to_string(m_KeyCode);
		}
		EVENT_CLASS_TYPE(KeyTyped)

	};
}
