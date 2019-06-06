#pragma once

#include "Utils.h"
#include "Vector.h"

class Engine 
{
public:
    constexpr auto getLocal() noexcept
    {
        return callVFunc<int>(this, 12);
    }

    constexpr auto setAngles(const Vector& viewAngles) noexcept
    {
        callVFunc<void, const Vector&>(this, 19, viewAngles);
    }

	constexpr auto maxClients() noexcept
	{
		return callVFunc<int>(this, 20);
	}

	constexpr auto inGame() noexcept
	{
		return callVFunc<bool>(this, 26);
	}
};