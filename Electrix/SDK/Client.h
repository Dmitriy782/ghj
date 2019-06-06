#pragma once

#include "Utils.h"

struct CClass;

class Client final 
{
public:
    constexpr auto getAll() noexcept
    {
        return callVFunc<CClass*>(this, 8);
    }
};