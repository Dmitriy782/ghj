#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Encryptor.h"

class Menu final 
{
public:
	void Draw() noexcept;
	bool isOpen{ false };
	void KeyCheck() noexcept;

private:
	struct 
	{
		struct
		{
			bool main{ false };

			bool subtab1{ true };
			bool subtab2{ false };
		} aimbot;

		struct
		{
			bool main{ false };
		} extras;
	} tab;

	std::string NewCFGName(std::string name = StrEncrypt("New CFG"), int i = 0) noexcept;
};

extern Menu menu;