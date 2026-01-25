#pragma once
#include "Core.h"
namespace Vectora {
	class VECTORA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	Application* CreateApplication();
}

