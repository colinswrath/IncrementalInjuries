#include "GlobalUIHandler.h"

Globals* Globals::GetSingleton()
{
	static Globals globals;
	return &globals;
}

void Globals::LoadGlobals()
{
	logger::info("LoadGlobals");
	auto globals = Globals::GetSingleton();
	
	auto hGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("HealthPenaltyUI");
	auto sGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("StaminaPenaltyUI");
	auto mGlobal = RE::TESGlobal::LookupByEditorID<RE::TESGlobal>("MagickaPenaltyUI");

	if (hGlobal && sGlobal && mGlobal)
	{
		globals->GlobalsFound = true;
		globals->HealthGlobal = hGlobal;
		globals->StaminaGlobal = sGlobal;
		globals->MagickaGlobal = mGlobal;
	}
}

void Globals::SetAVUIGlobal(RE::ActorValue av, float perc)
{
	auto globals = Globals::GetSingleton();
	if (globals->GlobalsFound)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth :
			globals->HealthGlobal->value = perc;
			break;
		case RE::ActorValue::kStamina :
			globals->StaminaGlobal->value = perc;
			break;
		case RE::ActorValue::kMagicka :
			globals->MagickaGlobal->value = perc;
			break;
		default:
			break;
		}
	}
}


