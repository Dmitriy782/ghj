#pragma once

#include <string_view>
#include <unordered_map>

#include "SDK/Recv.h"

class Netvars final 
{
public:
    Netvars() noexcept;

    auto operator[](const std::string_view netvar) const noexcept
    {
        return offsets[netvar];
    }

private:
    void Load(RecvTable*, const size_t = 0) noexcept;
    mutable std::unordered_map<std::string_view, size_t> offsets;
};

extern Netvars netvars;