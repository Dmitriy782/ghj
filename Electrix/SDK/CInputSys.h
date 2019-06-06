#pragma once

#include "Utils.h"

class CInputSys
{
public:
	constexpr void enableInput(bool enable) noexcept
	{
		callVFunc<void, bool>(this, 11, enable);
	}

	constexpr void resetInputState() noexcept
	{
		callVFunc<void>(this, 39);
	}
};