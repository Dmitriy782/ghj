#pragma once

#include <string_view>
#include <Windows.h>
#include <Psapi.h>

#include "SDK/Entity.h"
#include "SDK/CInput.h"
#include "SDK/GlobalVars.h"

class Memory final 
{
public:
	Memory() noexcept;

	uintptr_t present;
	uintptr_t reset;
	
	class ClientMode* clientMode;
	CInput* input;
	GlobalVars* globalVars;

	int* smokeCount;
	std::add_pointer_t<bool __cdecl(Vector, Vector, short) noexcept> lineGoesThroughSmoke;

private:
	template <typename T = uintptr_t>
	static auto findPattern(const char* module, std::string_view pattern, size_t offset = 0) noexcept
	{
		MODULEINFO mInfo;

		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &mInfo, sizeof(mInfo))) 
		{
			char* begin = static_cast<char*>(mInfo.lpBaseOfDll);
			char* end = begin + mInfo.SizeOfImage - pattern.length() + 1;

			for (char* c = begin; c != end; c++) 
			{
				bool matched = true;
				auto it = c;

				if (*(c + pattern.length() - 1) != pattern.back())
					continue;

				for (auto a : pattern) 
				{
					if (a != '?' && *it != a) 
					{
						matched = false;
						break;
					}
					it++;
				}
				if (matched)
					return reinterpret_cast<T>(c + offset);
			}
		}
		MessageBox(NULL, (std::ostringstream{ } << StrEncrypt("Failed to find pattern in ") << module << '!').str().c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
};

extern Memory mem;