#include <intrin.h>
#include <string>
#include <Windows.h>
#include <Psapi.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Config.h"
#include "Menu.h"
#include "Font.h"
#include "Hacks/Bhop.h"
#include "Hacks/Aimbot.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"
#include "SDK/UserCmd.h"
#include "Encryptor.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT __stdcall hkWndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if ((GetAsyncKeyState(VK_DELETE) & 1))
		menu.isOpen = !menu.isOpen;

	if (menu.isOpen)
		menu.KeyCheck();
	else
		ImGui::GetIO().MouseDown[0] = false;

	if (menu.isOpen && !ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	return CallWindowProc(hooks.oWndProc, window, msg, wParam, lParam);
}

static HRESULT __stdcall hkPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
{
	static bool menuInit{ ImGui_ImplDX9_Init(device) };

	static bool menuOpened{ false };

	if (menu.isOpen)
	{
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		IDirect3DVertexDeclaration9* vertexDeclaration;
		device->GetVertexDeclaration(&vertexDeclaration);

		if (!menuOpened)
		{
			mem.input->isMouseInitialized = false;
			mem.input->isMouseActive = false;
			interfaces.inputSys->enableInput(false);
			menuOpened = true;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		menu.Draw();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		device->SetVertexDeclaration(vertexDeclaration);
		vertexDeclaration->Release();
	}
	else if (menuOpened)
	{
		mem.input->isMouseInitialized = true;
		interfaces.inputSys->enableInput(true);
		interfaces.inputSys->resetInputState();
		menuOpened = false;
	}

	return hooks.oPresent(device, src, dest, windowOverride, dirtyRegion);
}

static HRESULT __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = hooks.oReset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

static bool __stdcall hkCreateMove(float sampleTime, UserCmd* userCmd) noexcept
{
	auto result = hooks.clientMode.oCall<bool, float, UserCmd*>(24, sampleTime, userCmd);

	if (!userCmd->command_number)
		return result;

	mem.globalVars->ServerTime(userCmd);
	Bhop::Do(userCmd);
	Aimbot::Do(userCmd);
	userCmd->viewangles.Normalize();
	userCmd->viewangles.x = std::clamp(userCmd->viewangles.x, -89.0f, 89.0f);
	userCmd->viewangles.y = std::clamp(userCmd->viewangles.y, -180.0f, 180.0f);
	userCmd->viewangles.z = 0.0f;

	return false;
}

void RenderMenuStyle() noexcept
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.08f, 0.10f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.00f, 0.00f, 0.90f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 0.08f, 0.10f, 0.51f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.25f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.00f, 0.08f, 0.20f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.00f, 0.08f, 0.25f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.00f, 0.08f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.31f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.86f, 0.00f, 0.08f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.43f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.45f, 0.00f, 0.00f, 0.37f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.82f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.04f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.71f, 0.04f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.04f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.04f, 0.00f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.75f, 0.02f, 0.00f, 0.78f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.86f, 0.02f, 0.00f, 0.73f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.02f, 0.00f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.00f, 0.00f, 0.71f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.78f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.88f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.78f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.86f, 0.00f, 0.00f, 0.65f);
	colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.94f, 0.04f, 0.00f, 0.76f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

	ImGuiStyle& style = ImGui::GetStyle();

	//Main
	style.WindowPadding = ImVec2(9.0f, 6.0f);
	style.FramePadding = ImVec2(7.0f, 3.0f);
	style.ItemSpacing = ImVec2(6.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 5.0f;
	style.ScrollbarSize = 13.0f;
	style.GrabMinSize = 3.0f;

	//Borders
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 1.0f;

	//Rounding
	style.WindowRounding = 1.0f;
	style.ChildRounding = 1.0f;
	style.FrameRounding = 0.0f;
	style.PopupRounding = 1.0f;
	style.ScrollbarRounding = 1.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 4.0f;

	//Alignment
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
	style.SelectableTextAlign = ImVec2(0.50f, 0.50f);

	//Safe Area Padding
	style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
}

void RenderMenu() noexcept
{
	ImGui::CreateContext();

	auto window = FindWindowA(StrEncrypt("Valve001"), NULL);
	ImGui_ImplWin32_Init(window);

	RenderMenuStyle();

	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	io.MouseDrawCursor = true;

	io.Fonts->AddFontFromMemoryTTF(SupremeFont, sizeof(SupremeFont), 16.0f);

	hooks.oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindowA(StrEncrypt("Valve001"), NULL), GWLP_WNDPROC, LONG_PTR(hkWndProc)));
}

Hooks::Hooks() noexcept
{
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

	RenderMenu();

	oPresent = **reinterpret_cast<decltype(oPresent) * *>(mem.present);
	**reinterpret_cast<void***>(mem.present) = reinterpret_cast<void*>(hkPresent);
	oReset = **reinterpret_cast<decltype(oReset) * *>(mem.reset);
	**reinterpret_cast<void***>(mem.reset) = reinterpret_cast<void*>(hkReset);

    clientMode.Hook(24, hkCreateMove);
}

uintptr_t* Hooks::VTable::freePage(void* const base, size_t vmtSize) noexcept
{
	MEMORY_BASIC_INFORMATION memInfo;
	VirtualQuery(base, &memInfo, sizeof(memInfo));

	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(memInfo.AllocationBase), &moduleInfo, sizeof(moduleInfo));
	uintptr_t* endModule{ reinterpret_cast<uintptr_t*>(static_cast<std::byte*>(moduleInfo.lpBaseOfDll) + moduleInfo.SizeOfImage) };

	for (auto curAdr = endModule - vmtSize; curAdr > moduleInfo.lpBaseOfDll; curAdr -= *curAdr ? vmtSize : 1)
		if (!*curAdr)
			if (VirtualQuery(curAdr, &memInfo, sizeof(memInfo)) && memInfo.State == MEM_COMMIT
				&& memInfo.Protect == PAGE_READWRITE && memInfo.RegionSize >= vmtSize * sizeof(uintptr_t)
				&& std::all_of(curAdr, curAdr + vmtSize, [](uintptr_t a) { return !a; }))
				return curAdr;

	return nullptr;
}

auto Hooks::VTable::calcLenght(uintptr_t* VT) noexcept
{
    size_t length{ 0 };
    MEMORY_BASIC_INFORMATION memInfo;

    while (VirtualQuery(reinterpret_cast<LPCVOID>(VT[length]), &memInfo, sizeof(memInfo)) && memInfo.Protect == PAGE_EXECUTE_READ)
        length++;

    return length;
}

Hooks::VTable::VTable(void* const base) noexcept
{
	this->base = base;

	oldVTable = *reinterpret_cast<uintptr_t * *>(base);
	length = calcLenght(oldVTable) + 1;

	if (newVTable = freePage(base, length)) 
	{
		std::copy(oldVTable - 1, oldVTable - 1 + length, newVTable);
		*reinterpret_cast<uintptr_t**>(base) = newVTable + 1;
	}
}