#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <Windows.h>

#include "SDK/Client.h"
#include "SDK/Cvar.h"
#include "SDK/Engine.h"
#include "SDK/EntList.h"
#include "SDK/CInputSys.h"

#include "Encryptor.h"

class Interfaces final 
{
public:
    Interfaces() noexcept;

    Client* client;
    Cvar* cvar;
    Engine* engine;
	EntList* entList;
	CInputSys* inputSys;

private:
    template <typename T>
	static auto moduleFind(const char* module, const char* name)
    {
		const auto createInterface = reinterpret_cast<std::add_pointer_t<T* (const char* name, int* returnCode)>>(GetProcAddress(GetModuleHandleA(module), "CreateInterface"));

		T* foundInterface{ nullptr };

		if (createInterface)
			foundInterface = createInterface(name, nullptr);

		if (foundInterface)
            return foundInterface;
        else
		{
			MessageBox(NULL, (std::ostringstream{ } << StrEncrypt("Failed to find ") << name << StrEncrypt(" in ") << module << '!').str().c_str(), "Error", MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}
    }
};

extern Interfaces interfaces;