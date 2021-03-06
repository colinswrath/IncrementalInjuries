#pragma once
#include "PlayerAV.h"
#include "GlobalUIHandler.h"
#include "PlayerAVDamageManager.h"

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 1,
		kOw = 'OWDA',
		kDamageValues = 'DAVA'
	};

	/*
	inline constexpr std::uint32_t kSerializationVersion = 1;
	inline constexpr std::uint32_t kOw = 'OWDA';
	inline constexpr std::uint32_t kDamageValues = 'DAVA';
	*/

	inline void UpdateGlobal(RE::ActorValue actorValue)
	{
		auto damageTracker = PlayerAV::ActorValueDamage::GetSingleton();
		auto damage = damageTracker->GetAVDamage(actorValue);
		auto player = RE::PlayerCharacter::GetSingleton();

		Globals::SetAVUIGlobal(actorValue, (damage / (damageTracker->GetActorValueMax(player, actorValue) + damage)) * 100.00f);
	}

	inline void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		auto playerAv = PlayerAV::ActorValueDamage::GetSingleton();

		if (!playerAv->SerializeSave(a_intfc, Serialization::kDamageValues, Serialization::kSerializationVersion))
		{
			logger::error("Failed to save damage values!\n");
		}
	}

	inline void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		auto playerAv = PlayerAV::ActorValueDamage::GetSingleton();

		uint32_t type;
		uint32_t version;
		uint32_t length;
		bool loaded = false;
		while (a_intfc->GetNextRecordInfo(type, version, length))
		{
			if (version != Serialization::kSerializationVersion)
			{
				logger::error("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, Serialization::kSerializationVersion, type);
				continue;
			}

			switch (type)
			{
			case Serialization::kDamageValues:
				if (!playerAv->DeserializeLoad(a_intfc))
				{
					logger::info("Failed to load damage values!\n");
				}
				else
				{
					loaded = true;
				}
				break;
			default:
				logger::error(FMT_STRING("Unrecognized signature type! {}"), type);
				break;
			}
		}

		if (loaded)
		{
			UpdateGlobal(RE::ActorValue::kStamina);
			UpdateGlobal(RE::ActorValue::kMagicka);
			UpdateGlobal(RE::ActorValue::kHealth);
		}
	}

	template<typename T>
	bool Save(SKSE::SerializationInterface* a_intfc, std::vector<T> setting)
	{
		std::size_t size = setting.size();
		if (!a_intfc->WriteRecordData(size))
		{
			logger::error("Failed to write size of record data!");
			return false;
		}
		else
		{
			for (auto& elem : setting)
			{
				if (!a_intfc->WriteRecordData(elem))
				{
					logger::error("Failed to write data for warmth elem!");
					return false;
				}
				logger::info("Serialized: " + std::to_string(elem));

			}
		}
		return true;
	}

	template<typename T>
	bool Load(SKSE::SerializationInterface* a_intfc, std::vector<T>& returnVector)
	{
		std::size_t size;
		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load size!");
			return false;
		}

		for (std::size_t i = 0; i < size; ++i)
		{
			T elem;
			if (!a_intfc->ReadRecordData(elem))
			{
				logger::error("Failed to load setting element!");
				return false;
			}
			else
			{
				logger::info("Deserialized: " + std::to_string(elem));
				returnVector.push_back(elem);
			}
		}
		return true;
	}

	inline void RevertCallback(SKSE::SerializationInterface*)
	{
		auto playerAv = PlayerAV::ActorValueDamage::GetSingleton();
		playerAv->Revert();
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void RevertCallback(SKSE::SerializationInterface*);
}
