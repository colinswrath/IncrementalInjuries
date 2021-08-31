#pragma once

class PlayerAVDamageManager
{
public:
	static void InstallHook();
	static void RestorePlayerAV(RE::ActorValue actorValue);
	static void SetAccumulators(RE::ActorValue actorValue, float val);

private:
	PlayerAVDamageManager() = default;
	static void DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken);
	static float PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2);
	inline static REL::Relocation<decltype(PlayerCheckClamp)> _PlayerCheckClamp;
};
