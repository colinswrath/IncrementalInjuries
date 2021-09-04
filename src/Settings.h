#pragma once
#include <SimpleIni.h>


class Settings
{
public:
	inline static float HealthDamageMult;
	inline static float HealthDamageLimit;
	inline static float StaminaDamageMult;
	inline static float StaminaDamageLimit;
	inline static float MagickaDamageMult;
	inline static float MagickaDamageLimit;

	inline static float GetDamageMult(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return HealthDamageMult;
			break;
		case RE::ActorValue::kStamina:
			return StaminaDamageMult;
			break;
		case RE::ActorValue::kMagicka:
			return MagickaDamageMult;
			break;
		default:
			logger::error("GetDamageMult case invalid.");
			return 0.0;
			break;
		}
	}

	inline static float GetDamageLimit(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return HealthDamageLimit;
			break;
		case RE::ActorValue::kStamina:
			return StaminaDamageLimit;
			break;
		case RE::ActorValue::kMagicka:
			return MagickaDamageLimit;
			break;
		default:
			logger::error("GetDamageLimit case invalid.");
			return 0.0;
			break;
		}
	}

	inline static void SetDamageMult(RE::ActorValue av, float mult)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			if (mult >= 0.0 && mult <= 1.0)
				HealthDamageMult = mult;
			break;
		case RE::ActorValue::kStamina:
			if (mult >= 0.0 && mult <= 1.0)
				StaminaDamageMult = mult;
			break;
		case RE::ActorValue::kMagicka:
			if (mult >= 0.0 && mult <= 1.0)
				MagickaDamageMult = mult;
			break;
		default:
			logger::error("SetDamageMult case invalid.");
			break;
		}
	}

	inline static void SetDamageLimit(RE::ActorValue av, float limit)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			if (limit >= 0.0 && limit <= 1.0)
				HealthDamageLimit = limit;
			break;
		case RE::ActorValue::kStamina:
			if (limit >= 0.0 && limit <= 1.0)
				StaminaDamageLimit = limit;
			break;
		case RE::ActorValue::kMagicka:
			if (limit >= 0.0 && limit <= 1.0)
				MagickaDamageLimit = limit;
			break;
		default:
			logger::error("SetDamageLimit case invalid.");
			break;
		}
	}

	inline static void LoadSettings()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\IncrementalInjuries.ini)");

		auto hMultIni = (float)ini.GetDoubleValue("Health", "fHealthDamageMult", 0.1);
		auto hLimIni = (float)ini.GetDoubleValue("Health", "fHealthDamageLimit", 0.5);
		auto sMultIni = (float)ini.GetDoubleValue("Stamina", "fStaminaDamageMult", 0.02);
		auto sLimIni = (float)ini.GetDoubleValue("Stamina", "fStaminaDamageLimit", 0.5);
		auto mMultIni = (float)ini.GetDoubleValue("Magicka", "fMagickaDamageMult", 0.02);
		auto mLimIni = (float)ini.GetDoubleValue("Magicka", "fMagickaDamageLimit", 0.5);

		//Validation
		(hMultIni < 0.0 || hMultIni > 1.0) ? HealthDamageMult = 0.1f : HealthDamageMult = hMultIni;
		(hLimIni < 0.0 || hLimIni > 1.0) ? HealthDamageLimit = 0.5f : HealthDamageLimit = hLimIni;
		(sMultIni < 0.0 || sMultIni > 1.0) ? StaminaDamageMult = 0.03f : StaminaDamageMult = sMultIni;
		(sLimIni < 0.0 || sLimIni > 1.0) ? StaminaDamageLimit = 0.5f : StaminaDamageLimit = sLimIni;
		(mMultIni < 0.0 || mMultIni > 1.0) ? MagickaDamageMult = 0.03f : MagickaDamageMult = mMultIni;
		(mLimIni < 0.0 || mLimIni > 1.0) ? MagickaDamageLimit = 0.5f : MagickaDamageLimit = mLimIni;
	}
};
