#include "Offsets.h"
#include "PlayerAVDamageManager.h"
#include "PlayerAV.h"
#include "Settings.h"
#include "GlobalUIHandler.h"

void PlayerAVDamageManager::InstallHook()
{
	static REL::Relocation<std::uintptr_t> PlayerCheckClampHook{ REL::ID(39602), 0x91 };
	auto& trampoline = SKSE::GetTrampoline();
	_PlayerCheckClamp = trampoline.write_branch<5>(PlayerCheckClampHook.address(), PlayerCheckClamp);
}

void PlayerAVDamageManager::InstallVHook()
{
	REL::Relocation<std::uintptr_t> healthUpdateSink{ REL::ID(517380) };
	_OnHealthUpdate = healthUpdateSink.write_vfunc(0, UpdateHealthSink);

	REL::Relocation<std::uintptr_t> staminaUpdateSink{ REL::ID(517382) };
	_OnStaminaUpdate = staminaUpdateSink.write_vfunc(0, UpdateStaminaSink);

	REL::Relocation<std::uintptr_t> magickaUpdateSink{ REL::ID(517381) };
	_OnMagickaUpdate = magickaUpdateSink.write_vfunc(0, UpdateMagickaSink);
}

void PlayerAVDamageManager::UpdateMagickaSink(RE::Actor* aActor, uint32_t aActorValue, float aOld, float aDelta)
{

	if (aActor == Player)
	{
		CheckAVLimit(Player, RE::ActorValue::kMagicka, aDelta);
	}

	_OnMagickaUpdate(aActor, aActorValue, aOld, aDelta);
}

void PlayerAVDamageManager::UpdateStaminaSink(RE::Actor* aActor, uint32_t aActorValue, float aOld, float aDelta)
{
	
	if (aActor == Player)
	{
		CheckAVLimit(Player, RE::ActorValue::kStamina, aDelta);
	}

	_OnStaminaUpdate(aActor, aActorValue, aOld, aDelta);
}

void PlayerAVDamageManager::UpdateHealthSink(RE::Actor* aActor, int32_t aActorValue, float aOld, float aDelta)
{

	if (aActor == Player)
	{
		CheckAVLimit(Player, RE::ActorValue::kHealth, aDelta);
	}
	_OnHealthUpdate(aActor,aActorValue, aOld, aDelta);
}


void PlayerAVDamageManager::CheckAVLimit(RE::PlayerCharacter* player, RE::ActorValue actorValue, float delta)
{
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
	auto damage = damageTracker->GetAVDamage(actorValue);

	if (delta < 0)
	{	
		float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
		float totalAV = currentMaxAV + damage;
		float avAtLimit = ceil(totalAV * Settings::GetDamageLimit(actorValue));
		float damageGoal = totalAV - avAtLimit;
	
		if (damage > (damageGoal))
		{
			damageTracker->SetAVAccumulator(actorValue, 0.0f);

			damageTracker->SetAVDamage(actorValue, damageGoal);
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, (avAtLimit - currentMaxAV));
		}
	}
	Globals::SetAVUIGlobal(actorValue, (damageTracker->GetAVDamage(actorValue) / (damageTracker->GetActorValueMax(player, actorValue) + damageTracker->GetAVDamage(actorValue))) * 100.00f);
}

float PlayerAVDamageManager::PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2)
{
	float damageRecieved = _PlayerCheckClamp(a1, a2);
	float posDamageFloat = damageRecieved * -1;
	if (posDamageFloat >= 0)
	{
		SetPlayerProperty(a1);
		RE::ActorValue damagedAv = RE::ActorValue(a2);
		switch (damagedAv)
		{
		case RE::ActorValue::kHealth:
			return damageRecieved + DamagePlayerAV(a1, RE::ActorValue::kHealth, posDamageFloat);	//Added together to prevent double loss of AV when decreasing
			break;
		case RE::ActorValue::kStamina:
			return damageRecieved + DamagePlayerAV(a1, RE::ActorValue::kStamina, posDamageFloat);
			break;
		case RE::ActorValue::kMagicka:
			return damageRecieved + DamagePlayerAV(a1, RE::ActorValue::kMagicka, posDamageFloat);
			break;
		default:
			break;
		}
	}

	return damageRecieved;
}

void PlayerAVDamageManager::SetAccumulators(RE::ActorValue actorValue, float val)
{
	PlayerAV::ActorValueDamage::GetSingleton()->SetAVAccumulator(actorValue, val);
}

void PlayerAVDamageManager::RestorePlayerAVAmount(RE::ActorValue actorValue, float amount)
{
	SetPlayerProperty(RE::PlayerCharacter::GetSingleton());
	auto avTracker = PlayerAV::ActorValueDamage::GetSingleton();
	Player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, amount);

	avTracker->SetAVDamage(actorValue, avTracker->GetAVDamage(actorValue) - amount);
}

void PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue actorValue)
{
	SetPlayerProperty(RE::PlayerCharacter::GetSingleton());
	auto avTracker = PlayerAV::ActorValueDamage::GetSingleton();
	Player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, avTracker->GetAVDamage(actorValue));

	avTracker->SetAVDamage(actorValue,0.0f);
}


float PlayerAVDamageManager::DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken)
{
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();

	damageTracker->SetAVAccumulator(actorValue, damageTracker->GetAVAccumulator(actorValue) + (damageTaken * Settings::GetDamageMult(actorValue)));

	float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
	float totalAV = damageTracker->GetTotalAVWithDamage(actorValue);

	float avAtLimit = ceil(totalAV*Settings::GetDamageLimit(actorValue));
	float damageGoal = totalAV - avAtLimit;
	
	float delta = 0.0f;
	if (damageTracker->GetAVAccumulator(actorValue) >= 1)
	{
		delta = floorf(damageTracker->GetAVAccumulator(actorValue));
		damageTracker->SetAVAccumulator(actorValue, 0.0f);
		
		//If max is already below the limit then continue on.
		if (currentMaxAV < avAtLimit)	
		{
			return 0;
		}
		//Check to see if we will push AV below the limit. 
		else if ((currentMaxAV - delta) <= avAtLimit)	
		{
			delta = damageGoal - damageTracker->GetAVDamage(actorValue);
			damageTracker->SetAVDamage(actorValue, damageGoal);
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue,-1 * delta);
		}
		else
		{
			damageTracker->SetAVDamage(actorValue, damageTracker->GetAVDamage(actorValue) + delta);
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, -1 * delta);
		}

	}
	return delta;
}

void PlayerAVDamageManager::SetPlayerProperty(RE::PlayerCharacter* playerCharacter)
{
	if(Player == NULL || Player != playerCharacter)
	{
		Player = playerCharacter;
	}
}



