#include <thread>
#include <chrono>
#include <string>
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Menu.h"
#include "Config.h"
#include "Encryptor.h"
#include "Hooks.h"
#include "SDK/Cvar.h"

void Menu::KeyCheck() noexcept //scuffed way to check if alt tabbing/taking a screenshot
{
	if ((GetAsyncKeyState(VK_MENU) & 0x8000))
	{
		if (GetAsyncKeyState(VK_TAB) & 1)
		{
			menu.isOpen = false;
		}
	}

	if ((GetAsyncKeyState(VK_F12) & 1) || (GetAsyncKeyState(VK_SNAPSHOT) & 1))
	{
		menu.isOpen = false;
	}
}

std::string Menu::NewCFGName(std::string name, int i) noexcept 
{
	std::vector<std::string> cfgItems = cfg.getCFGs();

	auto cName = std::find(cfgItems.begin(), cfgItems.end(), name + (i == 0 ? StrEncrypt("") : StrEncrypt(" ") + std::to_string(i)));

	if (cName == cfgItems.end())
		if (i == 0)
			return name;
		else
			return name + StrEncrypt(" ") + std::to_string(i);
	else
		return NewCFGName(name, i + 1);
}

void Menu::Draw() noexcept
{
	constexpr auto windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

	if (menu.isOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500.0f, 300.0f));

		ImGui::Begin(StrEncrypt("Electrix - It's a paste, but it's better than yours!"), &menu.isOpen, windowFlags);
		{
			ImGui::BeginChild(StrEncrypt("##Global Menu Child"), ImVec2(481.0f, 235.0f), true, windowFlags);
			{
				if (tab.aimbot.main)
				{
					ImGui::Checkbox(StrEncrypt("Aimbot##Master Switch"), &cfg.aimbot.active);
					ImGui::Separator();

					if (cfg.aimbot.active)
					{
						const char* gun_list[] =
						{
							StrEncrypt("None"),
							StrEncrypt("Glock-18"),
							StrEncrypt("P2000"),
							StrEncrypt("USP-S"),
							StrEncrypt("Dual Berettas"),
							StrEncrypt("P250"),
							StrEncrypt("Tec-9"),
							StrEncrypt("Five-Seven"),
							StrEncrypt("CZ-75"),
							StrEncrypt("Desert Eagle"),
							StrEncrypt("Revolver"),
							StrEncrypt("Nova"),
							StrEncrypt("XM1014"),
							StrEncrypt("Sawed-off"),
							StrEncrypt("MAG-7"),
							StrEncrypt("M249"),
							StrEncrypt("Negev"),
							StrEncrypt("Mac-10"),
							StrEncrypt("MP9"),
							StrEncrypt("MP7"),
							StrEncrypt("MP5-SD"),
							StrEncrypt("UMP-45"),
							StrEncrypt("P90"),
							StrEncrypt("PP-Bizon"),
							StrEncrypt("Galil AR"),
							StrEncrypt("Famas"),
							StrEncrypt("AK-47"),
							StrEncrypt("M4A4"),
							StrEncrypt("M4A1-S"),
							StrEncrypt("SSG-08"),
							StrEncrypt("SG-553"),
							StrEncrypt("AUG"),
							StrEncrypt("AWP"),
							StrEncrypt("G3SG1"),
							StrEncrypt("SCAR-20")
						};

						static int gun{ 0 };

						ImGui::BeginChild(StrEncrypt("##Aimbot Guns Child"), ImVec2(119.0f, 197.0f), false, windowFlags);
						{
							ImGui::PushItemWidth(119.0f);

							ImGui::ListBox(StrEncrypt("##Aimbot Gun List"), &gun, gun_list, IM_ARRAYSIZE(gun_list), 9);
						}
						ImGui::EndChild();

						ImGui::SameLine();

						ImGui::BeginChild(StrEncrypt("##Aimbot Child"), ImVec2(350.0f, 191.0f), false, windowFlags);
						{
							if (ImGui::Button(StrEncrypt("Main##Aimbot Subtab 1"), ImVec2(166.0f, 30.0f)))
							{
								tab.aimbot.subtab1 = true;
								tab.aimbot.subtab2 = false;
							}

							ImGui::SameLine();

							if (ImGui::Button(StrEncrypt("Other##Aimbot Subtab 2"), ImVec2(166.0f, 30.0f)))
							{
								tab.aimbot.subtab1 = false;
								tab.aimbot.subtab2 = true;
							}

							if (tab.aimbot.subtab1)
							{
								if (gun > 0)
								{
									ImGui::Columns(2, nullptr, false);
									ImGui::SetColumnOffset(1, 190.0f);

									ImGui::Checkbox(StrEncrypt("Enabled##For Current Gun"), &cfg.aimbot.guns[gun].enabled);

									if (cfg.aimbot.guns[gun].enabled)
									{
										const char* hitbox_list[] =
										{
											StrEncrypt("Nearest"),
											StrEncrypt("Head"),
											StrEncrypt("Neck"),
											StrEncrypt("Sternum"),
											StrEncrypt("Chest"),
											StrEncrypt("Stomach"),
											StrEncrypt("Pelvis")
										};

										ImGui::Checkbox(StrEncrypt("Silent (Client-Side)##Silent"), &cfg.aimbot.guns[gun].silent);

										ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

										ImGui::Text(StrEncrypt("Flags:"));
										ImGui::BeginChild(StrEncrypt("##Flags Child"), ImVec2(150.0f, 35.0f), true);
										{
											ImGui::Checkbox(StrEncrypt("Smoke##Smoke Check"), &cfg.aimbot.guns[gun].smokeCheck);
											ImGui::SameLine();
											ImGui::Checkbox(StrEncrypt("Flash##Flash Check"), &cfg.aimbot.guns[gun].flashCheck);
										}
										ImGui::EndChild();

										ImGui::NextColumn();

										ImGui::PushItemWidth(130.0f);

										ImGui::ListBox(StrEncrypt("##Hitbox"), &cfg.aimbot.guns[gun].hitbox, hitbox_list, IM_ARRAYSIZE(hitbox_list), 7);
									}
								}
								else
								{
									ImGui::Text(StrEncrypt("Please select a gun from the list!"));
								}
							}
							else if (tab.aimbot.subtab2)
							{
								if (cfg.aimbot.guns[gun].enabled)
								{
									ImGui::PushItemWidth(338.0f);

									ImGui::SliderFloat(StrEncrypt("##FOV"), &cfg.aimbot.guns[gun].fov, 0.0f, 30.0f, StrEncrypt("FOV: %.2f"));

									if (cfg.aimbot.guns[gun].fov > 30.0f)
										cfg.aimbot.guns[gun].fov = 30.0f;
									else if (cfg.aimbot.guns[gun].fov < 0.0f)
										cfg.aimbot.guns[gun].fov = 0.0f;

									ImGui::SliderFloat(StrEncrypt("##Smooth"), &cfg.aimbot.guns[gun].smooth, 1.0f, 100.0f, StrEncrypt("Smooth: %.2f"));

									if (cfg.aimbot.guns[gun].smooth > 100.0f)
										cfg.aimbot.guns[gun].smooth = 100.0f;
									else if (cfg.aimbot.guns[gun].smooth < 1.0f)
										cfg.aimbot.guns[gun].smooth = 1.0f;

									if (gun > 14 && gun != 29 && gun != 32)
									{
										ImGui::SliderFloat(StrEncrypt("##RCS X"), &cfg.aimbot.guns[gun].rcsX, 0.0f, 1.0f, StrEncrypt("RCS X: %.2f"));

										if (cfg.aimbot.guns[gun].rcsX > 1.0f)
											cfg.aimbot.guns[gun].rcsX = 1.0f;
										else if (cfg.aimbot.guns[gun].rcsX < 0.0f)
											cfg.aimbot.guns[gun].rcsX = 0.0f;

										ImGui::SliderFloat(StrEncrypt("##RCS Y"), &cfg.aimbot.guns[gun].rcsY, 0.0f, 1.0f, StrEncrypt("RCS Y: %.2f"));

										if (cfg.aimbot.guns[gun].rcsY > 1.0f)
											cfg.aimbot.guns[gun].rcsY = 1.0f;
										else if (cfg.aimbot.guns[gun].rcsY < 0.0f)
											cfg.aimbot.guns[gun].rcsY = 0.0f;
									}
								}
								else
								{
									if (gun <= 0)
										ImGui::Text(StrEncrypt("Please select a gun from the list!"));
									else
										ImGui::Text(StrEncrypt("Aimbot is not enabled for this gun!"));
								}
							}
							else
							{
								ImGui::Text(StrEncrypt("Please select a tab from above!"));
							}
						}
						ImGui::EndChild();
					}
				}
				else if (tab.extras.main)
				{
					constexpr auto& cfgItems = cfg.getCFGs();
					static int currCFG = -1;

					if (static_cast<size_t>(currCFG) >= cfgItems.size())
						currCFG = -1;

					static char buffer[16];
					 
					ImGui::BeginChild(StrEncrypt("##CFG System Child"), ImVec2(225.0f, 225.0f));
					{
						ImGui::PushItemWidth(180.0f);

						if (ImGui::ListBox(StrEncrypt("##CFG Selection"), &currCFG, [](void* data, int idx, const char** out_text)
						{
							auto& vector = *static_cast<std::vector<std::string>*>(data);
							*out_text = vector[idx].c_str();
							return true;
						}, &cfgItems, cfgItems.size(), 6) && currCFG != -1)
							strcpy_s(buffer, cfgItems[currCFG].c_str());

						if (ImGui::InputText(StrEncrypt("##CFG Name"), buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							if (currCFG != -1)
								cfg.SetName(currCFG, buffer);
						}

						if (ImGui::Button(StrEncrypt("Create##Create CFG"), ImVec2(91.5f, 25.0f)))
						{
							cfg.Add(buffer);
						}

						if (currCFG != -1)
						{
							ImGui::SameLine();

							if (ImGui::Button(StrEncrypt("Delete##Delete CFG"), ImVec2(91.5f, 25.0f)))
							{
								cfg.Remove(currCFG);
							}

							if (ImGui::Button(StrEncrypt("Save##Save CFG"), ImVec2(91.5f, 25.0f)))
							{
								cfg.Save(currCFG);
							}

							ImGui::SameLine();

							if (ImGui::Button(StrEncrypt("Load##Load CFG"), ImVec2(91.5f, 25.0f)))
							{
								cfg.Load(currCFG);
							}
						}
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild(StrEncrypt("##Extras Child"), ImVec2(225.0f, 225.0f));
					{
						ImGui::Checkbox(StrEncrypt("Bunny Hop"), &cfg.extras.bhop);
					}
					ImGui::EndChild();
				}
				else
				{
					ImGui::Text(StrEncrypt("Please select a tab from below!"));
				}
			}
			ImGui::EndChild();

			if (ImGui::Button(StrEncrypt("Aimbot##Extras Tab Trigger"), ImVec2(237.5f, 25.0f)))
			{
				tab.aimbot.main = true;
				tab.extras.main = false;
			}

			ImGui::SameLine();

			if (ImGui::Button(StrEncrypt("Extras##Aimbot Tab Trigger"), ImVec2(237.5f, 25.0f)))
			{
				tab.aimbot.main = false;
				tab.extras.main = true;
			}
		}
		ImGui::End();
	}
}