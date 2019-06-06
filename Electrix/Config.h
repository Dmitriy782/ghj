#pragma once

#include <filesystem>
#include "Interfaces.h"

class CFG final 
{
public:
	explicit CFG(const char*) noexcept;

	void Add(const char*) noexcept;
	void Remove(size_t) noexcept;
	void Save(size_t) const noexcept;
	void Load(size_t) noexcept;
	void SetName(size_t, const char*) noexcept;
	std::string currCfg;

	constexpr auto& getCFGs() noexcept
	{
		return cfgs;
	}

    struct
	{
		bool active{ true };

        struct
		{
            bool enabled{ false };
			bool silent{ false };
			bool smokeCheck{ false };
			bool flashCheck{ false };
			bool recoilBasedFov{ false };
			int hitbox{ 0 };
			float fov{ 0.0f };
            float smooth{ 1.0f };
            float rcsX{ 0.0f };
            float rcsY{ 0.0f };
        } guns[35];
    } aimbot;

	struct
	{
		bool bhop{ false };
	} extras;

private:
	std::filesystem::path path;
	std::vector<std::string> cfgs;
};

extern CFG cfg;