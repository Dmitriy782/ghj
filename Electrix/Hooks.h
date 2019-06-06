#pragma once

#include <d3d9.h>
#include <type_traits>

#include "Interfaces.h"
#include "Memory.h"

class Hooks final 
{
public:
    Hooks() noexcept;

	WNDPROC oWndProc;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*)> oPresent;
	std::add_pointer_t<HRESULT __stdcall(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*)> oReset;

    class VTable 
	{
    public:
        explicit VTable(void* const) noexcept;

        template<typename T>
        void Hook(size_t index, T fun) const noexcept
        {
            if (index <= length)
				newVTable[index + 1] = reinterpret_cast<uintptr_t>(fun);
        }

		template<typename T, typename ...Args>
		constexpr auto oCall(size_t index, Args... args) const noexcept
		{
			return reinterpret_cast<T(__thiscall*)(void*, Args...)>(oldVTable[index])(base, args...);
		}

    private:
		static uintptr_t* freePage(void* const, size_t) noexcept;
		static auto calcLenght(uintptr_t*) noexcept;
		void* base;
        uintptr_t* oldVTable;
        uintptr_t* newVTable;
        size_t length;
    };
	
	VTable clientMode{ mem.clientMode };
};

extern Hooks hooks;