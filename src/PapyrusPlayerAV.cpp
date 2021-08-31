#include "PlayerAVDamageManager.h"
#include "Settings.h"

namespace PapyrusPlayerAV
{
	void SetHealthMult(RE::StaticFunctionTag*, float mult)
	{
		auto settings = Settings::GetSingleton();
		settings->SetDamageMult(RE::ActorValue::kHealth, mult);
	}

	void SetStaminaMult(RE::StaticFunctionTag*, float mult)
	{
		auto settings = Settings::GetSingleton();
		settings->SetDamageMult(RE::ActorValue::kStamina, mult);
	}

	void SetMagickaMult(RE::StaticFunctionTag*, float mult)
	{
		auto settings = Settings::GetSingleton();
		settings->SetDamageMult(RE::ActorValue::kMagicka, mult);
	}



}
