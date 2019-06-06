#pragma once

#include "Utils.h"

class Entity;

class EntList 
{
public:
    constexpr auto getEnt(int entIndex) noexcept
    {
        return callVFunc<Entity*, int>(this, 3, entIndex);
    }

    constexpr auto getEntHandle(int handle) noexcept
    {
        return callVFunc<Entity*, int>(this, 4, handle);
    }

    constexpr auto getHighestEntIndex() noexcept
    {
        return callVFunc<int>(this, 6);
    }
};