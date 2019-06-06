#pragma once

#include "CClass.h"
#include "../Interfaces.h"
#include "../Netvars.h"
#include "Utils.h"
#include "Vector.h"
#include "WeaponId.h"
#include "../Encryptor.h"

enum class MoveType 
{
	LADDER = 9
};

class Entity 
{
public:

    template <typename T>
    constexpr auto getProperty(const std::string_view name, const size_t offset = 0) noexcept
    {
        return *reinterpret_cast<T*>(this + netvars[name] + offset);
    }

	template <typename T>
	constexpr void setProperty(const char* name, const T& value) noexcept
	{
		*reinterpret_cast<T*>(this + netvars[name]) = value;
	}

	auto getWeapons() noexcept
	{
		return reinterpret_cast<int*>(this + netvars[StrEncrypt("m_hActiveWeapon")] - 256);
	}

    using matrix3x4 = float[3][4];

    constexpr bool setupBones(matrix3x4* out, int maxBones, int boneMask, float currentTime) noexcept
    {
        return callVFunc<bool, matrix3x4*, int, int, float>(this + 4, 13, out, maxBones, boneMask, currentTime);
    }

    Vector getBonePos(int bone) noexcept
    {
        static matrix3x4 matrix[128];
        if (setupBones(matrix, 128, 256, 0.0f))
            return Vector{ matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3] };
        else
            return Vector{ };
    }

	constexpr Vector getEyePos() noexcept
    {	
		Vector vec{ };
        callVFunc<void, Vector&>(this, 281, vec);
        return vec;
    }

    bool isEnemy() noexcept
    {
        static auto gameType = interfaces.cvar->findVar(StrEncrypt("game_type"));

        if (!inLocalTeam() || gameType->getInt() == 6)
            return true;
        else
            return false;
    }

    constexpr bool isDormant() noexcept
    {
        return callVFunc<bool>(this + 8, 9);
    }

    constexpr CClass* getCClass() noexcept
    {
        return callVFunc<CClass*>(this + 8, 2);
    }

    constexpr bool isAlive() noexcept
    {
        return callVFunc<bool>(this, 153) && getProperty<int>(StrEncrypt("m_iHealth")) > 0;
    }

    constexpr bool inLocalTeam() noexcept
    {
        return callVFunc<bool>(this, 92);
    }
};