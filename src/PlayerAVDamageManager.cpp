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
			DamagePlayerAV(a1, RE::ActorValue::kHealth, posDamageFloat);
			break;
		case RE::ActorValue::kStamina:
			DamagePlayerAV(a1, RE::ActorValue::kStamina, posDamageFloat);
			break;
		case RE::ActorValue::kMagicka:
			DamagePlayerAV(a1, RE::ActorValue::kMagicka, posDamageFloat);
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
	player->ModActorValue(actorValue, PlayerAV::ActorValueDamage::GetSingleton()->GetAVDamage(actorValue));
	logger::info("Restored " + std::to_string(PlayerAV::ActorValueDamage::GetSingleton()->GetAVDamage(actorValue)));

	//auto restoreVal = player->GetPermanentActorValue(actorValue) + PlayerAV::ActorValueDamage::GetSingleton()->GetAVDamage(actorValue);

	//if (restoreVal > 0.0f)
	//{
	//	player->SetActorValue(actorValue, restoreVal);
	//}
}

void PlayerAVDamageManager::DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken)
{
	auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
	damageTracker->SetAVAccumulator(actorValue, damageTracker->GetAVAccumulator(actorValue) + (damageTaken * Settings::GetDamageMult(actorValue)));

	if (damageTracker->GetAVAccumulator(actorValue) >= 1)
	{
		float delta = floorf(damageTracker->GetAVAccumulator(actorValue));
		damageTracker->SetAVAccumulator(actorValue, 0.0f);
		
		float currentMaxAV = damageTracker->GetActorValueMax(player, actorValue);
		float totalAV = currentMaxAV + damageTracker->GetAVDamage(actorValue);
		float avAtLimit = (totalAV)*Settings::GetDamageLimit(actorValue);

		//If block checks for AV reduction limit
		if ((currentMaxAV - delta) <= avAtLimit)
		{
			damageTracker->SetAVDamage(actorValue, avAtLimit);
			player->SetActorValue(actorValue, avAtLimit);
		}
		else
		{
			damageTracker->SetAVDamage(actorValue, damageTracker->GetAVDamage(actorValue) + delta);
			player->ModActorValue(actorValue, -1 * delta);

		}

		Globals::SetAVUIGlobal(actorValue, (damageTracker->GetAVDamage(actorValue) / (totalAV)) * 100.00f);
	}
}
