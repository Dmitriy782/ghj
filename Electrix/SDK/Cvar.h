#pragma once

#include "Utils.h"

class Cvar 
{
public:
	constexpr auto getString() noexcept
	{
		return std::string_view{ callVFunc<const char*>(this, 11) };
	}

	constexpr float getFloat() noexcept
	{
		return callVFunc<float>(this, 12);
	}

	constexpr int getInt() noexcept
	{
		return callVFunc<int>(this, 13);
	}

	constexpr void setValue(const char* value) noexcept
	{
		callVFunc<void, const char*>(this, 14, value);
	}

	constexpr void setValue(float value) noexcept
	{
		callVFunc<void, float>(this, 15, value);
	}

	constexpr void setValue(int value) noexcept
	{
		callVFunc<void, int>(this, 16, value);
	}

	constexpr Cvar* findVar(const char* name) noexcept
	{
		return callVFunc<Cvar*, const char*>(this, 15, name);
	}

	std::byte pad[68];
	void* onChangeCallback;
};