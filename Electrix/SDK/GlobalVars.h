#pragma once

#include "Entity.h"
#include "UserCmd.h"

struct GlobalVars 
{
    const float realtime;
    const int framecount;
    const float absoluteFrameTime;
    const std::byte pad[4];
    const float currenttime;
    const float frametime;
    const int maxClients;
    const int tickCount;
    const float intervalPerTick;

    float ServerTime(UserCmd* = nullptr) noexcept;
};