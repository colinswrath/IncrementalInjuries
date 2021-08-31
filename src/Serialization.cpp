#include "Serialization.h"

namespace Serialization
{
	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		auto playerAv = PlayerAV::ActorValueDamage::GetSingleton();

		if (!playerAv->SerializeSave(a_intfc, Serialization::kDamageValues, Serialization::kSerializationVersion))
		{
			logger::error("Failed to save damage values!\n");
		}
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		auto playerAv = PlayerAV::ActorValueDamage::GetSingleton();

		uint32_t type;
		uint32_t version;
		uint32_t length;

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
					logger::error("Failed to load damage values!\n");
				}
				break;
			default:
				logger::error(FMT_STRING("Unrecognized signature type! {}"), type);
				break;
			}
		}
	}
}
