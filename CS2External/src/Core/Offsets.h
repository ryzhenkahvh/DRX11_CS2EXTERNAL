#pragma once
#include <cstdint>

namespace Offsets {
    // Основные оффсеты из CS2 Dumper
    constexpr auto dwEntityList = 0x1A36A00;
    constexpr auto dwLocalPlayerController = 0x1A88080;
    constexpr auto dwViewMatrix = 0x1AA27F0;

    namespace Entity {
        constexpr auto m_hPlayerPawn = 0x62C;
        constexpr auto m_iTeamNum = 0x3E3;
        constexpr auto m_iHealth = 0x344;
        constexpr auto m_vecOrigin = 0x88;
        constexpr auto m_szPlayerName = 0x660;
    }
}