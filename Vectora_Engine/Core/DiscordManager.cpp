#define DISCORDPP_IMPLEMENTATION
#include "DiscordManager.h"	
#include "vpch.h"

namespace Vectora {
	DiscordManager::DiscordManager()
	{
	}
	DiscordManager::~DiscordManager()
	{

	}
	void DiscordManager::Init()
	{
		s_Client = CreateRef<discordpp::Client>();
		s_Client->SetApplicationId(application_id);

		s_Client->AddLogCallback([](auto message, auto severity) {
			std::cout << "[" << EnumToString(severity) << "] " << message << std::endl;
			}, discordpp::LoggingSeverity::Info);

		s_IsReady = true;

		// Wait a tiny bit (or just call it directly) to ensure the pipe is open
		UpdatePresence("Vectora Engine", "Idle");

		
		//s_Client->Connect();
	}
	void DiscordManager::ShutDown()
	{
	}
	void DiscordManager::Update(float deltaTime)
	{
		discordpp::RunCallbacks();
		// Increment timer
		m_Timer += deltaTime;

		// Only send to Discord if 15s have passed AND we actually have new data to send
		if (m_Timer >= m_UpdateInterval)
		{
			if (m_NeedsUpdate && s_Client)
			{
				discordpp::Activity activity;
				activity.SetType(discordpp::ActivityTypes::Playing);
				activity.SetDetails(m_CachedDetails.c_str());
				activity.SetState(m_CachedState.c_str());

				s_Client->UpdateRichPresence(activity, [](const discordpp::ClientResult& result) {
					if (result.Successful()) {
						std::cout << "🎮 [Throttled] Rich Presence Updated.\n";
					}
					});

				m_NeedsUpdate = false; // Reset the flag
			}

			m_Timer = 0.0f; // Reset timer regardless to keep the 15s cadence
		}
	}
	void DiscordManager::UpdatePresence(const std::string& details, const std::string& state, bool isPlaying)
	{
		if (!s_Client) return;
		// If the data is exactly the same as what's currently showing, ignore it
		if (m_CachedDetails == details && m_CachedState == state) return;

		// Store the new data to be sent on the next "tick"
		m_CachedDetails = details;
		m_CachedState = state;
		m_NeedsUpdate = true;
	}
}
