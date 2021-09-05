#include "PapyrusPlayerAV.h"
#include "PlayerAVDamageManager.h"
#include "Settings.h"

namespace PapyrusPlayerAV
{
	/// <summary>
	/// Health = 1
	/// Stamina = 2
	/// Magicka = 3
	/// Send a negative value to decrease mult, positive to increase
	/// </summary>
	void ModActorValueMult(RE::StaticFunctionTag*, int actorValue, float value)
	{
		switch (actorValue)
		{
		case 1:
			Settings::ModDamageMult(RE::ActorValue::kHealth, value);
			break;
		case 2:
			Settings::ModDamageMult(RE::ActorValue::kStamina, value);
			break;
		case 3:
			Settings::ModDamageMult(RE::ActorValue::kMagicka, value);
			break;
		default:
			logger::error("ModActorValueMult: AV case not valid.");
			break;
		}
	}

	void RestoreActorValueDamage(RE::StaticFunctionTag*, int actorValue)
	{
		switch (actorValue)
		{
		case 1:
			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kHealth);
			break;
		case 2:
			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kStamina);
			break;
		case 3:
			PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue::kMagicka);
			break;
		default:
			logger::error("ModActorValueMult: AV case not valid.");
			break;
		}
	}

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

	float GetHealthMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kHealth);
	}

	float GetStaminaMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kStamina);
	}

	float GetMagickaMult(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageMult(RE::ActorValue::kMagicka);
	}

	float GetHealthLimit(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageLimit(RE::ActorValue::kHealth);
	}

	float GetStaminaLimit(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageLimit(RE::ActorValue::kStamina);
	}

	float GetMagickaLimit(RE::StaticFunctionTag*)
	{
		return Settings::GetDamageLimit(RE::ActorValue::kMagicka);
	}

	bool Register(VM* vm)
	{
		vm->RegisterFunction("ModActorValueMult", "IncrementalInjuries", ModActorValueMult);
		vm->RegisterFunction("RestoreActorValueDamage","IncrementalInjuries", RestoreActorValueDamage);
		vm->RegisterFunction("SetHealthMult", "IncrementalInjuries", SetHealthMult);
		vm->RegisterFunction("SetStaminaMult", "IncrementalInjuries", SetStaminaMult);
		vm->RegisterFunction("SetMagickaMult", "IncrementalInjuries", SetMagickaMult);
		vm->RegisterFunction("SetHealthLimit", "IncrementalInjuries", SetHealthLimit);
		vm->RegisterFunction("SetStaminaLimit", "IncrementalInjuries", SetStaminaLimit);
		vm->RegisterFunction("SetMagickaLimit", "IncrementalInjuries", SetMagickaLimit);
		vm->RegisterFunction("GetHealthMult", "IncrementalInjuries", GetHealthMult);
		vm->RegisterFunction("GetMagickaMult", "IncrementalInjuries", GetMagickaMult);
		vm->RegisterFunction("GetStaminaMult", "IncrementalInjuries", GetStaminaMult);
		vm->RegisterFunction("GetHealthLimit", "IncrementalInjuries", GetHealthLimit);
		vm->RegisterFunction("GetStaminaLimit", "IncrementalInjuries", GetStaminaLimit);
		vm->RegisterFunction("GetMagickaLimit", "IncrementalInjuries", GetMagickaLimit);


		logger::info("Registered IncrementalInjuries script");
		return true;
	}
}
