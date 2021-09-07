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

	REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(35565) , 0x1E };
	_OnFrameFunction = trampoline.write_call<5>(OnFrame_Update_Hook.address(), OnFrameUpdate);

	logger::info("Installed hook for frame update");
}

float PlayerAVDamageManager::PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2)
{
	float damageRecieved = _PlayerCheckClamp(a1, a2);
	float posDamageFloat = damageRecieved * -1;
	if (posDamageFloat >= 0)
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

//With every frame, check for AV damage being under the limit. Changes to total AV will not trigger the Damage clamp function.
//Other sources of AV damage may drive it under the limit, but we want to see if we caused it. If we did then bring our damage back up to the limit.
 
//For context: If we use a fortify health potion of 60 points at level 1, our health will raise to 160. Say we then take total av damage to the limit, making our total available health 80.
//When we lose the buff to 
std::int32_t PlayerAVDamageManager::OnFrameUpdate(std::int64_t a1)
{
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
	auto player = RE::PlayerCharacter::GetSingleton();

	CheckAVLimitOnFrame(player, damageTracker, RE::ActorValue::kHealth);
	CheckAVLimitOnFrame(player, damageTracker, RE::ActorValue::kStamina);
	CheckAVLimitOnFrame(player, damageTracker, RE::ActorValue::kMagicka);

	return _OnFrameFunction(a1);
}

void PlayerAVDamageManager::CheckAVLimitOnFrame(RE::PlayerCharacter* player, PlayerAV::ActorValueDamage* damageTracker, RE::ActorValue actorValue)
{
	float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
	float totalAV = currentMaxAV + damageTracker->GetAVDamage(actorValue);
	float avAtLimit = ceil(totalAV * Settings::GetDamageLimit(actorValue));
	float damageGoal = totalAV - avAtLimit;

	if (damageTracker->GetAVDamage(actorValue) > (damageGoal))
	{
		damageTracker->SetAVAccumulator(actorValue, 0.0f);
		//if player has had a health buff, and buff goes away we dont want to keep player damaged past limit if we caused it
		player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, (avAtLimit - currentMaxAV));
		damageTracker->SetAVDamage(actorValue, damageGoal);
	}
}

void PlayerAVDamageManager::SetAccumulators(RE::ActorValue actorValue, float val)
{
	PlayerAV::ActorValueDamage::GetSingleton()->SetAVAccumulator(actorValue, val);
}

void PlayerAVDamageManager::RestorePlayerAV(RE::ActorValue actorValue)
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto avTracker = PlayerAV::ActorValueDamage::GetSingleton();
	player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, PlayerAV::ActorValueDamage::GetSingleton()->GetAVDamage(actorValue));

	avTracker->SetAVDamage(actorValue,0.0f);
}

float PlayerAVDamageManager::DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken)
{
	float delta = 0.0f;
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();

	damageTracker->SetAVAccumulator(actorValue, damageTracker->GetAVAccumulator(actorValue) + (damageTaken * Settings::GetDamageMult(actorValue)));

	float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
	float totalAV = currentMaxAV + damageTracker->GetAVDamage(actorValue);
	float avAtLimit = ceil(totalAV*Settings::GetDamageLimit(actorValue));
	float damageGoal = totalAV - avAtLimit;

	if (damageTracker->GetAVAccumulator(actorValue) >= 1)
	{
		delta = floorf(damageTracker->GetAVAccumulator(actorValue));
		damageTracker->SetAVAccumulator(actorValue, 0.0f);
		
		if (currentMaxAV < avAtLimit)	//If max is already below the limit then continue on.
		{
			return 0;
		}
		else if ((currentMaxAV - delta) <= avAtLimit)	//Check to see if we will push AV below the limit. 
		{
			delta = damageGoal - damageTracker->GetAVDamage(actorValue);
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue,-1 * delta);
			damageTracker->SetAVDamage(actorValue, damageGoal);
		}
		else
		{
			player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, actorValue, -1 * delta);
			damageTracker->SetAVDamage(actorValue, damageTracker->GetAVDamage(actorValue) + delta);
		}

		Globals::SetAVUIGlobal(actorValue, (damageTracker->GetAVDamage(actorValue) / (totalAV)) * 100.00f);
	}
	return delta;
}
