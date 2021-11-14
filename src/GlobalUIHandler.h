#pragma once

class Globals
{
public:
	inline static bool GlobalsFound = false;
	inline static RE::TESGlobal* HealthGlobal;
	inline static RE::TESGlobal* StaminaGlobal;
	inline static RE::TESGlobal* MagickaGlobal;
	inline static std::string fileName = "IncrementalInjuries.esp";


	inline static void LoadGlobals()
	{
		logger::info("LoadGlobals");

		auto hGlobal = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(0x000800, fileName);
		auto sGlobal = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(0x000804, fileName);
		auto mGlobal = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(0x000806, fileName);

		if (hGlobal && sGlobal && mGlobal)
		{
			logger::info("Globals Loaded");

			GlobalsFound = true;
			HealthGlobal = hGlobal;
			StaminaGlobal = sGlobal;
			MagickaGlobal = mGlobal;
		}
		else
		{
			logger::info("Globals not Loaded");
		}
	}

	inline static void SetAVUIGlobal(RE::ActorValue av, float perc)
	{
		if (GlobalsFound)
		{
			switch (av)
			{
			case RE::ActorValue::kHealth:
				if (HealthGlobal)
				{
					HealthGlobal->value = perc;
				}
				break;
			case RE::ActorValue::kStamina:
				if (StaminaGlobal)
				{
					StaminaGlobal->value = perc;
				}
				break;
			case RE::ActorValue::kMagicka:
				if (MagickaGlobal)
				{
					MagickaGlobal->value = perc;
				}
				break;
			default:
				break;
			}
		}
	}
};


