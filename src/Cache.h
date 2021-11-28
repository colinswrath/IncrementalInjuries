#pragma once



namespace Cache
{

	class CachedAddresses
	{
	public:

		uintptr_t PlayerSingletonAddress;

		static CachedAddresses* GetSingleton()
		{
			static CachedAddresses addresses;
			return &addresses;
		}

		inline void SetPlayerSingletonAddress()
		{
			PlayerSingletonAddress = REL::ID(403521).address();
		}

		inline RE::PlayerCharacter* GetPlayerSingleton()
		{
			REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
			return singleton->get();
		}
	};
}

