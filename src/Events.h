#pragma once
#include "PlayerAVDamageManager.h"
#include "GlobalUIHandler.h"
#include "PlayerAV.h"
#include "Settings.h"

namespace Events
{
	static inline float Hours;

	inline static void RestoreAVDamagePercent(RE::ActorValue actorValue)
	{
		auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
		auto player = RE::PlayerCharacter::GetSingleton();

		auto healthRestoreValPerHour = ceilf(damageTracker->GetAVDamage(actorValue) * Settings::GetSleepPercent(actorValue));		//TODO, make temp mult for each need

		logger::info("Sleep restore/hr " + std::to_string(healthRestoreValPerHour));
		logger::info("Hours " + std::to_string(Hours));

		if (damageTracker->GetAVSleepRestore(actorValue) < healthRestoreValPerHour)
		{
			damageTracker->SetAVSleepRestore(actorValue, healthRestoreValPerHour);
		}

		auto totalRestoreVal = ceilf(Hours * damageTracker->GetAVSleepRestore(actorValue));

		if (totalRestoreVal >= damageTracker->GetAVDamage(actorValue))
		{
			totalRestoreVal = damageTracker->GetAVDamage(actorValue);
			damageTracker->SetAVSleepRestore(actorValue,0.0f);
		}
		logger::info("Sleep total restore " + std::to_string(totalRestoreVal));

		PlayerAVDamageManager::RestorePlayerAVAmount(actorValue, totalRestoreVal);
		Globals::SetAVUIGlobal(actorValue, (damageTracker->GetAVDamage(actorValue) / (damageTracker->GetActorValueMax(player, actorValue) + damageTracker->GetAVDamage(actorValue))) * 100.00f);
	}


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

			Hours = RE::Calendar::GetSingleton()->GetHoursPassed();

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

	class OnSleepStopEventHandler : public RE::BSTEventSink<RE::TESSleepStopEvent>
	{
	public:

		static OnSleepStopEventHandler* GetSingleton()
		{
			static OnSleepStopEventHandler singleton;
			return std::addressof(singleton);
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*) override
		{
			if (!a_event)
			{
				return RE::BSEventNotifyControl::kContinue;
			}

			Hours = RE::Calendar::GetSingleton()->GetHoursPassed() - Hours;
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kHealth, 0.0f);
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kStamina, 0.0f);
			PlayerAVDamageManager::SetAccumulators(RE::ActorValue::kMagicka, 0.0f);
			
			RestoreAVDamagePercent(RE::ActorValue::kHealth);
			RestoreAVDamagePercent(RE::ActorValue::kMagicka);
			RestoreAVDamagePercent(RE::ActorValue::kStamina);
			
			return RE::BSEventNotifyControl::kContinue;
		}

		static void Register()
		{
			RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventHolder->AddEventSink(OnSleepStopEventHandler::GetSingleton());
		}

	private:
		OnSleepStopEventHandler() = default;
	};


	inline static void Register()
	{
		OnSleepStartEventHandler::Register();
		OnSleepStopEventHandler::Register();
	}
}
