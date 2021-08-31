#pragma once

class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	float GetDamageMult(RE::ActorValue av);
	float GetDamageLimit(RE::ActorValue av);
	void SetDamageMult(RE::ActorValue av, float mult);
	void SetDamageLimit(RE::ActorValue av, float limit);

private:
	float HealthDamageMult;
	float HealthDamageLimit;
	float StaminaDamageMult;
	float StaminaDamageLimit;
	float MagickaDamageMult;
	float MagickaDamageLimit;
};
