#pragma once

#include "../Config.h"

enum class GunId : short 
{
    Deagle = 1,
    Elite,
    Fiveseven,
    Glock,
    Ak47 = 7,
    Aug,
    Awp,
    Famas,
    G3SG1,
    GalilAr = 13,
    M249,
    M4A1 = 16,
    Mac10,
    P90 = 19,
    Mp5sd = 23,
    Ump45,
    Xm1014,
    Bizon,
    Mag7,
    Negev,
    Sawedoff,
    Tec9,
    Taser,
    Hkp2000,
    Mp7,
    Mp9,
    Nova,
    P250,
    Scar20 = 38,
    Sg553,
    Ssg08,
	GoldenKnife,
    M4a1_s = 60,
    Usp_s,
    Cz75a = 63,
	Revolver,
	GhostKnife = 80,
	Bayonet = 500,
	Flip = 505,
	Gut,
	Karambit,
	M9Bayonet,
	Huntsman,
	Falchion = 512,
	Bowie = 514,
	Butterfly,
	Daggers,
	Ursus = 519,
	Navaja,
	Stiletto = 522,
	Talon
};

constexpr int getGunIndex(GunId gunId) noexcept
{
    switch (gunId) 
	{
    default:
		return 0;

    case GunId::Glock:
		return 1;

    case GunId::Hkp2000:
		return 2;

    case GunId::Usp_s:
		return 3;

    case GunId::Elite:
		return 4;

    case GunId::P250:
		return 5;

    case GunId::Tec9:
		return 6;

    case GunId::Fiveseven:
		return 7;

    case GunId::Cz75a:
		return 8;

    case GunId::Deagle:
		return 9;

    case GunId::Revolver:
		return 10;

    case GunId::Nova:
		return 11;

    case GunId::Xm1014:
		return 12;

    case GunId::Sawedoff:
		return 13;

    case GunId::Mag7:
		return 14;

    case GunId::M249:
		return 15;

    case GunId::Negev:
		return 16;

    case GunId::Mac10:
		return 17;

    case GunId::Mp9:
		return 18;

    case GunId::Mp7:
		return 19;

    case GunId::Mp5sd:
		return 20;

    case GunId::Ump45:
		return 21;

    case GunId::P90:
		return 22;

    case GunId::Bizon:
		return 23;

    case GunId::GalilAr:
		return 24;

    case GunId::Famas:
		return 25;

    case GunId::Ak47:
		return 26;

    case GunId::M4A1:
		return 27;

    case GunId::M4a1_s:
		return 28;

    case GunId::Ssg08:
		return 29;

    case GunId::Sg553:
		return 30;

    case GunId::Aug:
		return 31;

    case GunId::Awp:
		return 32;

    case GunId::G3SG1:
		return 33;

    case GunId::Scar20:
		return 34;
    }
}