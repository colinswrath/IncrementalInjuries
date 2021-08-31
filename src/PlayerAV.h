#pragma once

namespace PlayerAV
{
	class ActorValueDamage
	{
	public:
		static ActorValueDamage* GetSingleton()
		{
			static ActorValueDamage damageValues;
			return &damageValues;
		}

		float GetAVDamage(RE::ActorValue av);

		void SetAVDamage(RE::ActorValue av, float damage);

		float GetAVAccumulator(RE::ActorValue av);

		void SetAVAccumulator(RE::ActorValue av, float damage);

		float GetHealthDamage();

		void SetHealthDamage(float damage);

		float GetStaminaDamage();

		void SetStaminaDamage(float damage);

		float GetMagickaDamage();

		void SetMagickaDamage(float damage);


		bool SerializeSave(SKSE::SerializationInterface* a_intfc);
		bool SerializeSave(SKSE::SerializationInterface* a_intfc, uint32_t a_type, uint32_t a_version);
		bool DeserializeLoad(SKSE::SerializationInterface* a_intfc);


	private:

		ActorValueDamage()
		{
			_healthDamage = 0.0f;
			_staminaDamage = 0.0f;
			_magickaDamage = 0.0f;
			_healthAccumulator = 0.0f;
			_staminaAccumulator = 0.0f;
			_magickaAccumulator = 0.0f;
		}

		float _healthDamage;
		float _staminaDamage;
		float _magickaDamage;

		float _healthAccumulator;
		float _staminaAccumulator;
		float _magickaAccumulator;
	};
}
