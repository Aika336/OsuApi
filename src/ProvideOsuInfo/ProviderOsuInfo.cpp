#include "ProviderOsuInfo.h"
#include <OsuOffset.h>
#include "../../include/Memory/Memory.h"

std::optional<uintptr_t> ProvideOsuInfo::GetCurrentScreenAddress()
{
    uintptr_t screen_stack = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGame_ScreenStack).value();

    uintptr_t stack = Memory::RPM<uintptr_t>(handler_,
        screen_stack + OsuOffsets::ScreenStack_stack).value();

    uint32_t count = Memory::RPM<uint32_t>(handler_, stack + 0x10).value();

    uintptr_t items = Memory::RPM<uintptr_t>(handler_, stack + 0x8).value();

    if (count <= 0 || items == 0)
        return std::nullopt;

    return Memory::RPM<uintptr_t>(handler_, items + 0x10 + 0x8 * (count - 1));
}

std::optional<uint32_t> ProvideOsuInfo::GetCurrentCombo()
{
    uintptr_t score = Memory::RPM<uintptr_t>(handler_,
        current_screen + OsuOffsets::Player_ScoreProcessor).value();

    if (score == 0) return std::nullopt;

    uintptr_t combo = Memory::RPM<uintptr_t>(handler_,
        score + OsuOffsets::OsuScoreProcessor_Combo).value();
    if (combo == 0) return std::nullopt;

    return Memory::RPM<int32_t>(handler_, combo + 0x40);
}
