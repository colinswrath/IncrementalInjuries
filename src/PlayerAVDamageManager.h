#pragma once
#include "PlayerAV.h"

class PlayerAVDamageManager
{
public:
	static void InstallHook();
	static void InstallVHook();
	static void RestorePlayerAV(RE::ActorValue actorValue);
	static void SetAccumulators(RE::ActorValue actorValue, float val);

private:
	PlayerAVDamageManager() = default;
	static float DamagePlayerAV(RE::PlayerCharacter* player, RE::ActorValue actorValue, float damageTaken);
	static float PlayerCheckClamp(RE::PlayerCharacter* a1, int32_t a2);

	static void UpdateHealthSink(RE::Actor* aActor, int32_t aActorValue, float aOld, float aDelta);
	static void UpdateStaminaSink(RE::Actor* aActor, uint32_t aActorValue, float aOld, float aDelta);
	static void UpdateMagickaSink(RE::Actor* aActor, uint32_t aActorValue, float aOld, float aDelta);

	static void CheckAVLimit(RE::PlayerCharacter* player, PlayerAV::ActorValueDamage* damageTracker, RE::ActorValue actorValue);
	inline static REL::Relocation<decltype(PlayerCheckClamp)> _PlayerCheckClamp;

	inline static std::int32_t OnFrameUpdate(std::int64_t a1);
	inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

	inline static REL::Relocation<decltype(UpdateHealthSink)> _OnHealthUpdate;
	inline static REL::Relocation<decltype(UpdateStaminaSink)> _OnStaminaUpdate;
	inline static REL::Relocation<decltype(UpdateMagickaSink)> _OnMagickaUpdate;
};
