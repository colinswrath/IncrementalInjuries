#pragma once
#include "PlayerAVDamageManager.h"
#include "GlobalUIHandler.h"

namespace Events
{
	class OnSleepStartEventHandler : public RE::BSTEventSink<RE::TESSleepStartEvent>
	{
	public:
		static OnSleepStartEventHandler* GetSingleton()
		{
			static OnSleepStartEventHandler singleton;
			return std::addressof(singleton);
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStartEvent* a_event, RE::BSTEventSource<RE::TESSleepStartEvent>*) override
		{
			if (!a_event)
			{
				return RE::BSEventNotifyControl::kContinue;
			}

			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kHealth);
			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kStamina);
			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kMagicka);
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kHealth, 0.0f);
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kStamina, 0.0f);
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kMagicka, 0.0f);

			Globals::SetAVUIGlobal(RE::ActorValue::kHealth, 0.0f);
			Globals::SetAVUIGlobal(RE::ActorValue::kStamina, 0.0f);
			Globals::SetAVUIGlobal(RE::ActorValue::kMagicka, 0.0f);

			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnSleepStartEventHandler::GetSingleton());
		}

	private:
		OnSleepStartEventHandler() = default;
	};

	inline static void Register()
	{
		OnSleepStartEventHandler::Register();
	}
}
