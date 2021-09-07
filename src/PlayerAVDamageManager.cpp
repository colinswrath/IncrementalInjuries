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

float PlayerAVDamageManager::PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2)
{
	float damageRecieved = _PlayerCheckClamp(a1, a2);
	float posDamageFloat = damageRecieved * -1;
	if (posDamageFloat > 0)
	{
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

void PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue actorValue)
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto avTracker = PlayerAV::ActorValueDamage::GetSingleton();
	player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, actorValue, PlayerAV::ActorValueDamage::GetSingleton()->GetAVDamage(actorValue));

	avTracker->SetAVDamage(actorValue,0.0f);
}

float PlayerAVDamageManager::DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken)
{
	float delta = 0.0f;
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
	damageTracker->SetAVAccumulator(actorValue, damageTracker->GetAVAccumulator(actorValue) + (damageTaken * Settings::GetDamageMult(actorValue)));

	if (damageTracker->GetAVAccumulator(actorValue) >= 1)
	{
		delta = floorf(damageTracker->GetAVAccumulator(actorValue));
		damageTracker->SetAVAccumulator(actorValue, 0.0f);
		
		float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
		float totalAV = currentMaxAV + damageTracker->GetAVDamage(actorValue);

		float avAtLimit = ceil((totalAV)*Settings::GetDamageLimit(actorValue));

		//If block checks for AV reduction limit
		if ((currentMaxAV - delta) <= avAtLimit)
		{
			float damageGoal = totalAV - avAtLimit;
			//player->SetActorValue(actorValue, avAtLimit);
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, actorValue, -1 * (damageGoal - damageTracker->GetAVDamage(actorValue)));
			damageTracker->SetAVDamage(actorValue, damageGoal);
			return 0;
		}
		else
		{
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kTemporary, actorValue, -1 * delta);
			damageTracker->SetAVDamage(actorValue, damageTracker->GetAVDamage(actorValue) + delta);
		}

		Globals::SetAVUIGlobal(actorValue, (damageTracker->GetAVDamage(actorValue) / (totalAV)) * 100.00f);
	}
	return delta;
}
