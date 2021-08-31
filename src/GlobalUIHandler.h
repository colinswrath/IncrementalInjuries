#pragma once

class Globals
{
	public:
		static Globals* GetSingleton();
		static void SetAVUIGlobal(RE::ActorValue av, float perc);
		static void LoadGlobals();

		bool GlobalsFound = false;
		RE::TESGlobal* HealthGlobal;
		RE::TESGlobal* StaminaGlobal;
		RE::TESGlobal* MagickaGlobal;
};

