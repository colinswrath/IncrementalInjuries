#pragma once

namespace PapyrusPlayerAV
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;

	void SetHealthMult(RE::StaticFunctionTag*, float mult);
	void SetStaminaMult(RE::StaticFunctionTag*, float mult);
	void SetMagickaMult(RE::StaticFunctionTag*, float mult);
}
