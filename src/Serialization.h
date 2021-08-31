#pragma once
#include "PlayerAV.h"

namespace Serialization
{
	enum
	{
		kSerializationVersion = 1,
		kOw = 'OWDA',
		kDamageValues = 'DAVA'
	};

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

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);

}
