#pragma once

#include "discordpp.h"

#include "Core/Core.h"
#include <string>

namespace Vectora
{
	class DiscordManager
	{
	public:
		DiscordManager();
		~DiscordManager();

		static void Init();
		static void ShutDown();
		static void Update(float deltaTime);

		static void UpdatePresence(const std::string& details, const std::string& state, bool isPlaying = false);
	private:
		inline static VE_UINT64 application_id = 1483295673691541656;
		inline static Ref<discordpp::Client> s_Client = nullptr;
		//inline static std::shared_ptr<discordpp::Client> s_Client = nullptr;
		inline static bool s_IsReady = false;

		inline static float m_Timer = 0.0f;
		inline static const float m_UpdateInterval = 15.0f; // Discord's recommended limit
		inline static bool m_NeedsUpdate = false;

		// Cache to prevent redundant updates
		inline static std::string m_CachedDetails;
		inline static std::string m_CachedState;
	};
}

