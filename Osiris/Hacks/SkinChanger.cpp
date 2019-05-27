#include "../Interfaces.h"
#include "../SDK/Entity.h"
#include "SkinChanger.h"
#include "../Config.h"
#include "../nSkinz/SDK/CBaseEntity.hpp"

extern void post_data_update_start(sdk::C_BasePlayer*);

static bool hudUpdateRequired{ false };

void SkinChanger::run(FrameStage stage) noexcept
{
    if (stage == FrameStage::NET_UPDATE_POSTDATAUPDATE_START) {
        
        if (const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer()); localPlayer && localPlayer->isAlive()) {
            post_data_update_start(reinterpret_cast<sdk::C_BasePlayer*>(localPlayer));
            if (hudUpdateRequired && !localPlayer->isDormant()) {
                auto hudWeaponSelection = memory.findHudElement(memory.hud, "CCSGO_HudWeaponSelection");

                if (auto hud_weapons = hudWeaponSelection - 0xA0) {
                    for (int i = 0; i < *reinterpret_cast<int*>(hud_weapons + 0x80); i++)
                        i = memory.clearHudWeapon(hud_weapons, i);
                }
                hudUpdateRequired = false;
            }
        }
    }
}

void SkinChanger::scheduleHudUpdate() noexcept
{
    hudUpdateRequired = true;
}
