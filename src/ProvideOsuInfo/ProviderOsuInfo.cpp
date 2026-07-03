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
        GetCurrentScreenAddress().value() + OsuOffsets::Player_ScoreProcessor).value();

    if (score == 0) return std::nullopt;

    uintptr_t combo = Memory::RPM<uintptr_t>(handler_,
        score + OsuOffsets::OsuScoreProcessor_Combo).value();
    if (combo == 0) return std::nullopt;

    return Memory::RPM<int32_t>(handler_, combo + 0x40);
}

std::optional<bool> ProvideOsuInfo::GetPlayingState()
{
    uintptr_t game_api = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_API).value();
    uintptr_t game_score_manager = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_ScoreManager).value();
    uintptr_t player_api = Memory::RPM<uintptr_t>(handler_,
        GetCurrentScreenAddress().value() + OsuOffsets::Player_api).value();
    uintptr_t player_score_manager = Memory::RPM<uintptr_t>(handler_,
        GetCurrentScreenAddress().value() + OsuOffsets::Player_scoreManager).value();

    if (game_api == 0 || game_score_manager == 0 || player_api == 0 || player_score_manager == 0) {
        return std::nullopt;
    }

    return player_api == game_api && player_score_manager == game_score_manager;
}
