#include "PlayerAVDamageManager.h"
#include "Settings.h"

namespace PapyrusPlayerAV
{
	void SetHealthMult(RE::StaticFunctionTag*, float mult)
	{
		if (mult > 0 && mult <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kHealth, mult);
		}
	}

	void SetStaminaMult(RE::StaticFunctionTag*, float mult)
	{
		if (mult > 0 && mult <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kStamina, mult);
		}
	}

	void SetMagickaMult(RE::StaticFunctionTag*, float mult)
	{
		if (mult > 0 && mult <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kMagicka, mult);
		}
	}

	void SetHealthLimit(RE::StaticFunctionTag*, float limit)
	{
		if (limit > 0 && limit <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kHealth, limit);
		}
	}

	void SetStaminaLimit(RE::StaticFunctionTag*, float limit)
	{
		if (limit > 0 && limit <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kStamina, limit);
		}
	}

	void SetMagickaLimit(RE::StaticFunctionTag*, float limit)
	{
		if (limit > 0 && limit <= 1.0)
		{
			Settings::SetDamageMult(RE::ActorValue::kMagicka, limit);
		}
	}

	float getHealthMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kHealth);
	}

	float getStaminaMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kStamina);
	}

	float getMagickaMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kMagicka);
	}

	float getHealthLimit(RE::StaticFunctionTag*)
	{

	}

	float getStaminaLimit(RE::StaticFunctionTag*)
	{

	}

	float getMagickaLimit(RE::StaticFunctionTag*)
	{

	}
}
