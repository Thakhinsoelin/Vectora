#pragma once
#include "Events/Event.h"

namespace Vectora {
	class VECTORA_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {
		}
		int m_KeyCode;
	};

	class VECTORA_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, bool isRepeat = false)
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
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {
		}
		std::string ToString() const override {
			return std::string("KeyReleasedEvent: ") + std::to_string(m_KeyCode);
		}
		EVENT_CLASS_TYPE(KeyReleased)
	
	};
}
