#pragma once
#include "PlayerAV.h"

class PlayerAVDamageManager
{
public:
	static void InstallHook();
	static void RestorePlayerAV(RE::ActorValue actorValue);
	static void SetAccumulators(RE::ActorValue actorValue, float val);

private:
	PlayerAVDamageManager() = default;
	static float DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken);
	static float PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2);
	static void CheckAVLimitOnFrame(RE::PlayerCharacter* player, PlayerAV::ActorValueDamage* damageTracker, RE::ActorValue actorValue);
	inline static REL::Relocation<decltype(PlayerCheckClamp)> _PlayerCheckClamp;
	inline static std::int32_t OnFrameUpdate(std::int64_t a1);
	inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

};
