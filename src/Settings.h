#pragma once
#include <SimpleIni.h>


class Settings
{
public:
	inline static float HealthDamageMult;
	inline static float HealthDamageLimit;
	inline static float HealthSleepPerc;

	inline static float StaminaDamageMult;
	inline static float StaminaDamageLimit;
	inline static float StaminaSleepPerc;

	inline static float MagickaDamageMult;
	inline static float MagickaDamageLimit;
	inline static float MagickaSleepPerc;

	inline static float MultMax = 5.0;

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
			if (mult >= 0.0 && mult <= MultMax)
				HealthDamageMult = mult;
			break;
		case RE::ActorValue::kStamina:
			if (mult >= 0.0 && mult <= MultMax)
				StaminaDamageMult = mult;
			break;
		case RE::ActorValue::kMagicka:
			if (mult >= 0.0 && mult <= MultMax)
				MagickaDamageMult = mult;
			break;
		default:
			logger::error("SetDamageMult case invalid.");
			break;
		}
	}

	inline static void ModDamageMult(RE::ActorValue av, float value)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			if ((Settings::GetDamageMult(RE::ActorValue::kHealth) + value) >= 0 || (Settings::GetDamageMult(RE::ActorValue::kHealth) <= MultMax))
				HealthDamageMult = HealthDamageMult + value;
			else if ((Settings::GetDamageMult(RE::ActorValue::kHealth) + value) > MultMax)
				HealthDamageMult = MultMax;
			else
				HealthDamageMult = 0;
			break;
		case RE::ActorValue::kStamina:
			if ((Settings::GetDamageMult(RE::ActorValue::kStamina) + value) >= 0 || (Settings::GetDamageMult(RE::ActorValue::kStamina) <= MultMax))
				StaminaDamageMult = StaminaDamageMult + value;
			else if ((Settings::GetDamageMult(RE::ActorValue::kStamina) + value) > MultMax)
				StaminaDamageMult = MultMax;
			else
				StaminaDamageMult = 0;
			break;
		case RE::ActorValue::kMagicka:
			if ((Settings::GetDamageMult(RE::ActorValue::kMagicka) + value) >= 0 || (Settings::GetDamageMult(RE::ActorValue::kMagicka) <= MultMax))
				MagickaDamageMult = MagickaDamageMult + value;
			else if ((Settings::GetDamageMult(RE::ActorValue::kMagicka) + value) > MultMax)
				StaminaDamageMult = MultMax;
			else
				StaminaDamageMult = 0;
			break;
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

	inline static float GetSleepPercent(RE::ActorValue av)
	{
		switch (av)
		{
		case RE::ActorValue::kHealth:
			return HealthSleepPerc;
			break;
		case RE::ActorValue::kStamina:
			return StaminaSleepPerc;
			break;
		case RE::ActorValue::kMagicka:
			return MagickaSleepPerc;
			break;
		default:
			logger::error("GetDamageLimit case invalid.");
			return 0.0;
			break;
		}
	}

	inline static void LoadSettings()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\IncrementalInjuries.ini)");

		auto hMultIni = (float)ini.GetDoubleValue("Health", "fHealthDamageMult", 0.05);
		auto hLimIni = (float)ini.GetDoubleValue("Health", "fHealthDamageLimit", 0.5);
		auto hSleepIni = (float)ini.GetDoubleValue("Health", "fHealthPercRestorePerHour", 0.25);

		auto sMultIni = (float)ini.GetDoubleValue("Stamina", "fStaminaDamageMult", 0.02);
		auto sLimIni = (float)ini.GetDoubleValue("Stamina", "fStaminaDamageLimit", 0.5);
		auto sSleepIni = (float)ini.GetDoubleValue("Stamina", "fStaminaPercRestorePerHour", 0.25);


		auto mMultIni = (float)ini.GetDoubleValue("Magicka", "fMagickaDamageMult", 0.02);
		auto mLimIni = (float)ini.GetDoubleValue("Magicka", "fMagickaDamageLimit", 0.5);
		auto mSleepIni = (float)ini.GetDoubleValue("Magicka", "fMagickaPercRestorePerHour", 0.25);


		//Validation
		(hMultIni < 0.0 || hMultIni > 5.0) ? HealthDamageMult = 0.05f : HealthDamageMult = hMultIni;
		(hLimIni < 0.0 || hLimIni > 1.0) ? HealthDamageLimit = 0.5f : HealthDamageLimit = hLimIni;
		(hSleepIni < 0.0 || hSleepIni > 1.0) ? HealthSleepPerc = 0.25f : HealthSleepPerc = hSleepIni;

	
		(sMultIni < 0.0 || sMultIni > 5.0) ? StaminaDamageMult = 0.02f : StaminaDamageMult = sMultIni;
		(sLimIni < 0.0 || sLimIni > 1.0) ? StaminaDamageLimit = 0.5f : StaminaDamageLimit = sLimIni;
		(sSleepIni < 0.0 || sSleepIni > 1.0) ? StaminaSleepPerc = 0.25f : StaminaSleepPerc = sSleepIni;

		(mMultIni < 0.0 || mMultIni > 5.0) ? MagickaDamageMult = 0.02f : MagickaDamageMult = mMultIni;
		(mLimIni < 0.0 || mLimIni > 1.0) ? MagickaDamageLimit = 0.5f : MagickaDamageLimit = mLimIni;
		(mSleepIni < 0.0 || mSleepIni > 1.0) ? MagickaSleepPerc = 0.25f : MagickaSleepPerc = mSleepIni;
	}
};
