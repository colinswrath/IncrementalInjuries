#include "PlayerAV.h"
#include "Serialization.h"

namespace PlayerAV
{
	float ActorValueDamage::GetAVDamage(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return _healthDamage;
			break;
		case RE::ActorValue::kStamina:
			return _staminaDamage;
			break;
		case RE::ActorValue::kMagicka:
			return _magickaDamage;
			break;
		default:
			logger::error("GetAVDamage case not valid.");
			return 0.0f;
			break;
		}
	}

	void ActorValueDamage::SetAVDamage(RE::ActorValue av, float damage)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			_healthDamage = damage;
			break;
		case RE::ActorValue::kStamina:
			_staminaDamage = damage;
			break;
		case RE::ActorValue::kMagicka:
			_magickaDamage = damage;
			break;
		default:
			logger::error("SetAVDamage case not valid.");
			break;
		}
	}

	float ActorValueDamage::GetAVAccumulator(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return _healthAccumulator;
			break;
		case RE::ActorValue::kStamina:
			return _staminaAccumulator;
			break;
		case RE::ActorValue::kMagicka:
			return _magickaAccumulator;
			break;
		default:
			logger::error("GetAVDelta case not valid.");
			return 0.0f;
			break;
		}
		
	}

	void ActorValueDamage::SetAVAccumulator(RE::ActorValue av, float damage)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			_healthAccumulator = damage;
			break;
		case RE::ActorValue::kStamina:
			_staminaAccumulator = damage;
			break;
		case RE::ActorValue::kMagicka:
			_magickaAccumulator = damage;
			break;
		default:
			logger::error("SetAVDelta case not valid.");
			break;
		}
	}

	bool ActorValueDamage::SerializeSave(SKSE::SerializationInterface* a_intfc)
	{
		auto damageVals = PlayerAV::ActorValueDamage::GetSingleton();

		std::vector<float> damageValues = { damageVals->_healthDamage, damageVals->_staminaDamage, damageVals->_magickaDamage, damageVals->_healthAccumulator, damageVals->_staminaAccumulator, damageVals->_magickaAccumulator };	//Prepare vector for serialization

		if(!Serialization::Save(a_intfc, damageValues))
		{
			logger::error("Failed to write damage values");
			return false;
		}
		return true;
	}

	bool ActorValueDamage::SerializeSave(SKSE::SerializationInterface* a_intfc, uint32_t a_type, uint32_t a_version)
	{
		if (!a_intfc->OpenRecord(a_type, a_version))
		{
			logger::error("Failed to open damage values record!");
			return false;
		}
		else
		{
			return SerializeSave(a_intfc);
		}
	}

	bool ActorValueDamage::DeserializeLoad(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<float> damageValues;
		auto playerAVDamage = PlayerAV::ActorValueDamage::GetSingleton();

		if (!Serialization::Load(a_intfc, damageValues))
		{
			logger::info("Failed to load damage values!");
			return false;
		}

		playerAVDamage->_healthDamage = damageValues[0];
		playerAVDamage->_staminaDamage = damageValues[1];
		playerAVDamage->_magickaDamage = damageValues[2];
		playerAVDamage->_healthAccumulator = damageValues[3];
		playerAVDamage->_staminaAccumulator = damageValues[4];
		playerAVDamage->_magickaAccumulator = damageValues[5];

		return true;
	}
	
	void ActorValueDamage::Revert()
	{
		auto playerAVDamage = PlayerAV::ActorValueDamage::GetSingleton();

		playerAVDamage->_healthDamage = 0.0f;
		playerAVDamage->_staminaDamage = 0.0f;
		playerAVDamage->_magickaDamage = 0.0f;
		playerAVDamage->_healthAccumulator = 0.0f;
		playerAVDamage->_staminaAccumulator = 0.0f;
		playerAVDamage->_magickaAccumulator = 0.0f;
	}

	float ActorValueDamage::GetTotalAVWithDamage(RE::ActorValue av)
	{
		float currentMaxAV = GetActorValueMax(RE::PlayerCharacter::GetSingleton(), av);
		return currentMaxAV + GetAVDamage(av);
	}

	float ActorValueDamage::GetAVSleepRestore(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return _sleepRestoreHealthAmount;
			break;
		case RE::ActorValue::kStamina:
			return _sleepRestoreStaminaAmount;
			break;
		case RE::ActorValue::kMagicka:
			return _sleepRestoreMagickaAmount;
			break;
		default:
			logger::error("case not valid.");
			return 0.0f;
			break;
		}
	}

	void ActorValueDamage::SetAVSleepRestore(RE::ActorValue av, float amount)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			_sleepRestoreHealthAmount = amount;
			break;
		case RE::ActorValue::kStamina:
			_sleepRestoreStaminaAmount = amount;
			break;
		case RE::ActorValue::kMagicka:
			_sleepRestoreMagickaAmount = amount;
			break;
		default:
			logger::error("case not valid.");
			break;
		}
	}
	
}
