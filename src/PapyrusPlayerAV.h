#pragma once

namespace PapyrusPlayerAV
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;

	/// <summary>
	/// Health = 1
	/// Stamina = 2
	/// Magicka = 3
	/// </summary>
	void ModActorValueMult(RE::StaticFunctionTag*, int actorValue, float value);
	void RestoreActorValueDamage(RE::StaticFunctionTag*, int actorValue);

	void SetHealthMult(RE::StaticFunctionTag*, float mult);
	void SetStaminaMult(RE::StaticFunctionTag*, float mult);
	void SetMagickaMult(RE::StaticFunctionTag*, float mult);

	void SetHealthLimit(RE::StaticFunctionTag*, float limit);
	void SetStaminaLimit(RE::StaticFunctionTag*, float limit);
	void SetMagickaLimit(RE::StaticFunctionTag*, float limit);

	float GetHealthMult(RE::StaticFunctionTag*);
	float GetStaminaMult(RE::StaticFunctionTag*);
	float GetMagickaMult(RE::StaticFunctionTag*);

	float GetHealthLimit(RE::StaticFunctionTag*);
	float GetStaminaLimit(RE::StaticFunctionTag*);
	float GetMagickaLimit(RE::StaticFunctionTag*);

	bool Register(VM* vm);
}
