#pragma once

class Globals
{
public:
	inline static bool GlobalsFound = false;
	inline static RE::TESGlobal* HealthGlobal;
	inline static RE::TESGlobal* StaminaGlobal;
	inline static RE::TESGlobal* MagickaGlobal;


	inline static void LoadGlobals()
	{
		logger::info("LoadGlobals");

		auto hGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("HealthPenaltyUI");
		auto sGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("StaminaPenaltyUI");
		auto mGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("MagickaPenaltyUI");

		if (hGlobal && sGlobal && mGlobal)
		{
			GlobalsFound = true;
			HealthGlobal = hGlobal;
			StaminaGlobal = sGlobal;
			MagickaGlobal = mGlobal;
		}
	}

	inline static void SetAVUIGlobal(RE::ActorValue av, float perc)
	{
		if (GlobalsFound)
		{
			switch (av)
			{
			case RE::ActorValue::kHealth:
				HealthGlobal->value = perc;
				break;
			case RE::ActorValue::kStamina:
				StaminaGlobal->value = perc;
				break;
			case RE::ActorValue::kMagicka:
				MagickaGlobal->value = perc;
				break;
			default:
				break;
			}
		}
	}
};


