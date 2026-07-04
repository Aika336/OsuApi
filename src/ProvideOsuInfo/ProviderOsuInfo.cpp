#include "ProviderOsuInfo.h"
#include <OsuOffset.h>
#include "../../include/Memory/Memory.h"
#include <DotNetString.h>
#include "algorithm"
#include "iostream"

int a = 0;

std::optional<uintptr_t> ProvideOsuInfo::GetCurrentScreenAddress()
{
    auto screen_stack = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGame_ScreenStack);

    auto stack = Memory::RPM<uintptr_t>(handler_,
        *screen_stack + OsuOffsets::ScreenStack_stack);

    auto count = Memory::RPM<uint32_t>(handler_, *stack + 0x10);

    auto items = Memory::RPM<uintptr_t>(handler_, *stack + 0x8);


    if (count <= 0 || items == 0 || !screen_stack || !stack || !count || !items)
        return std::nullopt;

    return Memory::RPM<uintptr_t>(handler_, *items + 0x10 + 0x8 * (*count - 1));
}

std::optional<uintptr_t> ProvideOsuInfo::GetScoreInfo()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return std::nullopt;
    }

    auto player_score = Memory::RPM<uintptr_t>(handler_, screen.value() + OsuOffsets::Player_Score);
    if (!player_score) {
        return std::nullopt;
    }

    auto score_info = Memory::RPM<uintptr_t>(handler_, *player_score + 0x8);
    if (!player_score) {
        return std::nullopt;
    }

	return score_info == 0 ? std::nullopt : std::optional<uintptr_t>(score_info);
}

std::vector<std::string> ProvideOsuInfo::GetCurrentMods()
{

    std::vector<std::string> mods;
    auto score_info = GetScoreInfo();
    if (!score_info || !GetPlayingState())
        return mods;

    auto string_address = Memory::RPM<uintptr_t>(handler_, score_info.value() + OsuOffsets::ScoreInfo_ModsJson);
    if (!string_address)
        return mods;

    std::wstring mods_json = DotNetString::Read(handler_, *string_address).value_or(L"");

    if (mods_json.empty()) return mods;

    const std::wstring keys[] = { L"\"acronym\":\"", L"\"Acronym\":\"" };

    for (const std::wstring& key : keys) {
        size_t pos = 0;
        while ((pos = mods_json.find(key, pos)) != std::wstring::npos) {
            pos += key.length();
            size_t end = mods_json.find(L"\"", pos);
            if (end == std::wstring::npos)
                break;

            std::wstring acronym_w = mods_json.substr(pos, end - pos);
            std::string acronym(acronym_w.begin(), acronym_w.end());

            if (!acronym.empty() && std::find(mods.begin(), mods.end(), acronym) == mods.end())
                mods.push_back(acronym);

            pos = end + 1;
        }
    }

    return mods;
}

int ProvideOsuInfo::GetCurrentCombo()
{
    if (!GetPlayingState()) return 1;

    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return -1;
    }

    auto score = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_ScoreProcessor);

    if (!score) return -1;

    auto combo = Memory::RPM<uintptr_t>(handler_,
        *score + OsuOffsets::OsuScoreProcessor_Combo);
    if (!combo) return -1;

    auto combo_value = Memory::RPM<int>(handler_, combo.value() + 0x40);
    if (!combo_value) return -1;

    return *combo_value;
}

bool ProvideOsuInfo::GetPlayingState()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return false;
    }

    auto game_api = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_API);
    auto game_score_manager = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_ScoreManager);
    auto player_api = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_api);
    auto player_score_manager = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_scoreManager);

    if (!game_api || !game_score_manager || !player_api || !player_score_manager) {
        return false;
    }

    return player_api == game_api && player_score_manager == game_score_manager;
}
