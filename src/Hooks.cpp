#include "Hooks.h"
#include "PlayerAVDamageManager.h"

namespace Hooks
{
	void Install()
	{
		PlayerAVDamageManager::InstallHook();
		logger::info("Damage hook installed.");
	}
}


	

