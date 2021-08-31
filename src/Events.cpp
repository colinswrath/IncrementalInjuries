#include "Events.h"
#include "PlayerAVDamageManager.h"
#include "GlobalUIHandler.h"

namespace Events
{
	RE::BSEventNotifyControl OnSleepStopEventHandler::ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*)
	{
		if (!a_event)
		{
			return RE::BSEventNotifyControl::kContinue;
		}
		
		PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kHealth);
		PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kStamina);
		PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kMagicka);
		PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kHealth,0.0f);
		PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kStamina,0.0f);
		PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kMagicka,0.0f);

		auto globals = Globals::GetSingleton();
		globals->SetAVUIGlobal(RE::ActorValue::kHealth, 0.0f);
		globals->SetAVUIGlobal(RE::ActorValue::kStamina, 0.0f);
		globals->SetAVUIGlobal(RE::ActorValue::kMagicka, 0.0f);

		return RE::BSEventNotifyControl::kContinue;
	}

	void Register()
	{
		OnSleepStopEventHandler::Register();
	}
}
