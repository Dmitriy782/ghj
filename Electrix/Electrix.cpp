#include "Config.h"
#include "Menu.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"
#include "Netvars.h"
#include "Encryptor.h"

CFG cfg{ StrEncrypt("Electrix") };
Menu menu;
Interfaces interfaces;
Memory mem;
Netvars netvars;
Hooks hooks;