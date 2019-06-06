#include <stdexcept>
#include <Windows.h>

#include "Interfaces.h"
#include "Memory.h"
#include "Encryptor.h"

Memory::Memory() noexcept
{
	present = findPattern<>(StrEncrypt("gameoverlayrenderer"), StrEncrypt("\xFF\x15????\x8B\xF8\x85\xDB"), 2);
	reset = findPattern<>(StrEncrypt("gameoverlayrenderer"), StrEncrypt("\xC7\x45?????\xFF\x15????\x8B\xF8"), 9);
	clientMode = **reinterpret_cast<ClientMode***>((*reinterpret_cast<uintptr_t**>(interfaces.client))[10] + 5);
	input = *reinterpret_cast<CInput**>((*reinterpret_cast<uintptr_t**>(interfaces.client))[16] + 1);
	globalVars = **reinterpret_cast<GlobalVars***>((*reinterpret_cast<uintptr_t**>(interfaces.client))[11] + 10);
	lineGoesThroughSmoke = findPattern<decltype(lineGoesThroughSmoke)>(StrEncrypt("client_panorama"), StrEncrypt("\x55\x8B\xEC\x83\xEC\x08\x8B\x15????\x0F\x57\xC0"));
	smokeCount = *(reinterpret_cast<int**>(lineGoesThroughSmoke) + 2);
}