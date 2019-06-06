#include "../Config.h"
#include "../Memory.h"
#include "Bhop.h"

void Bhop::Do(UserCmd* userCmd) noexcept
{
	if (cfg.extras.bhop)
	{
		auto pLocal = interfaces.entList->getEnt(interfaces.engine->getLocal());

		if (!(pLocal->getProperty<int>(StrEncrypt("m_fFlags")) & 1) && pLocal->getProperty<MoveType>(StrEncrypt("m_nRenderMode"), 1) != MoveType::LADDER)
			userCmd->buttons &= ~UserCmd::IN_JUMP;
	}
}