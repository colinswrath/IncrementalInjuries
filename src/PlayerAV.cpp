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
			logger::error("Failed to load damage values!");
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


	
}
