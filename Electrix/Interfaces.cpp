#include "Interfaces.h"
#include "Encryptor.h"

Interfaces::Interfaces() noexcept
{
    client = moduleFind<Client>(StrEncrypt("client_panorama"), StrEncrypt("VClient018"));
    cvar = moduleFind<Cvar>(StrEncrypt("vstdlib"), StrEncrypt("VEngineCvar007"));
    engine = moduleFind<Engine>(StrEncrypt("engine"), StrEncrypt("VEngineClient014"));
	entList = moduleFind<EntList>(StrEncrypt("client_panorama"), StrEncrypt("VClientEntityList003"));
	inputSys = moduleFind<CInputSys>(StrEncrypt("inputsystem"), StrEncrypt("InputSystemVersion001"));
}