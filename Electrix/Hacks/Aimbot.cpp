#define _USE_MATH_DEFINES
#include <cmath>

#include "Aimbot.h"
#include "../Config.h"
#include "../Interfaces.h"
#include "../Memory.h"
#include "../SDK/Entity.h"
#include "../SDK/UserCmd.h"
#include "../SDK/Vector.h"
#include "../SDK/WeaponId.h"
#include "../SDK/GlobalVars.h"

static Vector calcAngleBetween(const Vector& src, const Vector& dst, const Vector& vAngle) noexcept
{
    Vector calcDelta = dst - src;

    constexpr auto radToDeg = [](float rad) noexcept { return rad * 180 / static_cast<float>(M_PI); };

    Vector relativeAngles{ radToDeg(atan2f(-calcDelta.z, std::hypotf(calcDelta.x, calcDelta.y))) - vAngle.x, radToDeg(atan2f(calcDelta.y, calcDelta.x)) - vAngle.y };

	relativeAngles.Normalize();

    return relativeAngles;
}

void Aimbot::Do(UserCmd* userCmd) noexcept
{
	const auto pLocal = interfaces.entList->getEnt(interfaces.engine->getLocal());
	
	if (pLocal->getProperty<float>(StrEncrypt("m_flNextAttack")) > mem.globalVars->ServerTime())
		return;
	
	const auto currGun = interfaces.entList->getEntHandle(pLocal->getProperty<int>(StrEncrypt("m_hActiveWeapon")));

	if (!currGun || !currGun->getProperty<int>(StrEncrypt("m_iClip1")))
		return;

	auto gun = getGunIndex(currGun->getProperty<GunId>(StrEncrypt("m_iItemDefinitionIndex")));

	if (!gun)
		return;

	if (!cfg.aimbot.guns[gun].enabled)
		gun = 0;

	if (cfg.aimbot.guns[gun].flashCheck)
		if (pLocal->getProperty<float>(StrEncrypt("m_flFlashDuration")))
			return;

	if (cfg.aimbot.active)
	{
		if (cfg.aimbot.guns[gun].enabled)
		{
			if (userCmd->buttons & UserCmd::IN_ATTACK)
			{
				auto bestFov = cfg.aimbot.guns[gun].fov;

				Vector bestTarget{ };

				auto pLocalEyePos = pLocal->getEyePos();

				static auto gunRecoil = interfaces.cvar->findVar(StrEncrypt("weapon_recoil_scale"));
				auto punchAngle = pLocal->getProperty<Vector>(StrEncrypt("m_aimPunchAngle")) * gunRecoil->getFloat();
				punchAngle.x *= cfg.aimbot.guns[gun].rcsY;
				punchAngle.y *= cfg.aimbot.guns[gun].rcsX;

				for (int i = 1; i <= interfaces.engine->maxClients(); i++)
				{
					auto entity = interfaces.entList->getEnt(i);

					if (!entity
						|| entity == pLocal
						|| entity->isDormant()
						|| !entity->isAlive()
						|| !entity->isEnemy()
						|| entity->getProperty<bool>(StrEncrypt("m_bGunGameImmunity")))
						continue;

					for (int j = 8; j >= 3; j--)
					{
						auto bonePos = entity->getBonePos(cfg.aimbot.guns[gun].hitbox ? 9 - cfg.aimbot.guns[gun].hitbox : j);
						auto angle = calcAngleBetween(pLocalEyePos, bonePos, userCmd->viewangles + (cfg.aimbot.guns[gun].recoilBasedFov ? punchAngle : Vector{ }));
						auto fov = std::hypotf(angle.x, angle.y);

						if (gun > 14 && gun != 7 && gun != 29 && gun != 32 || gun == 0)
							cfg.aimbot.guns[gun].recoilBasedFov = false;
						else
							cfg.aimbot.guns[gun].recoilBasedFov = true;

						if (fov < bestFov)
						{
							bestFov = fov;
							bestTarget = bonePos;
						}

						if (cfg.aimbot.guns[gun].hitbox)
							break;
					}
				}

				if (bestTarget)
				{
					if (!cfg.aimbot.guns[gun].smokeCheck || !mem.lineGoesThroughSmoke(pLocalEyePos, bestTarget, 1))
					{
						if (userCmd->buttons & UserCmd::IN_ATTACK)
						{
							auto angle = calcAngleBetween(pLocal->getEyePos(), bestTarget, userCmd->viewangles + punchAngle);
							angle /= cfg.aimbot.guns[gun].smooth;
							userCmd->viewangles += angle;

							if (!cfg.aimbot.guns[gun].silent || cfg.aimbot.guns[gun].smooth > 1.0f)
								interfaces.engine->setAngles(userCmd->viewangles);
						}
					}
				}
			}
		}
	}
}
