#pragma once
#include "Events/Event.h"


namespace Vectora {
	class VECTORA_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {
		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override final
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE_FINAL(WindowResize)
		EVENT_CLASS_CATEGORY_FINAL(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class VECTORA_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE_FINAL(WindowClose)
		EVENT_CLASS_CATEGORY_FINAL(EventCategoryApplication)
	};

	class VECTORA_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE_FINAL(AppTick)
		EVENT_CLASS_CATEGORY_FINAL(EventCategoryApplication)
	};

	class VECTORA_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE_FINAL(AppUpdate)
		EVENT_CLASS_CATEGORY_FINAL(EventCategoryApplication)
	};

	class VECTORA_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE_FINAL(AppRender)
		EVENT_CLASS_CATEGORY_FINAL(EventCategoryApplication)
	};
}
