#include "GlobalVars.h"

float GlobalVars::ServerTime(UserCmd* userCmd) noexcept
{
    static int tick;
    static UserCmd* lastCmd;

    if (userCmd)
	{
        if (!lastCmd || lastCmd->hasbeenpredicted)
            tick = interfaces.entList->getEnt(interfaces.engine->getLocal())->getProperty<int>(StrEncrypt("m_nTickBase"));
        else
            tick++;

        lastCmd = userCmd;
    }
    return tick * intervalPerTick;
}