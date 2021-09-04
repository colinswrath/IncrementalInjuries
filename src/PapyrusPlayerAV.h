#pragma once

namespace PapyrusPlayerAV
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;

	void SetHealthMult(RE::StaticFunctionTag*, float mult);
	void SetStaminaMult(RE::StaticFunctionTag*, float mult);
	void SetMagickaMult(RE::StaticFunctionTag*, float mult);

	void SetHealthLimit(RE::StaticFunctionTag*, float limit);
	void SetStaminaLimit(RE::StaticFunctionTag*, float limit);
	void SetMagickaLimit(RE::StaticFunctionTag*, float limit);

	float getHealthMult(RE::StaticFunctionTag*);
	float getStaminaMult(RE::StaticFunctionTag*);
	float getMagickaMult(RE::StaticFunctionTag*);

	float getHealthLimit(RE::StaticFunctionTag*);
	float getStaminaLimit(RE::StaticFunctionTag*);
	float getMagickaLimit(RE::StaticFunctionTag*);
}
