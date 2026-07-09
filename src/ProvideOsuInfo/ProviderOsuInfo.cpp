#include "OsuOffset.h"
#include "DotNetString.h"
#include "ProviderOsuInfo.h"
#include "Memory.h"
#include "Logger.h"

#include <algorithm>
#include <iostream>

std::optional<uintptr_t> OsuInfoProvider::GetCurrentScreenAddress()
{
    auto screen_stack = Memory::ReadAs<uintptr_t>(handle_,
        base_address_ + OsuOffsets::OsuGame_ScreenStack);

    auto stack = Memory::ReadAs<uintptr_t>(handle_,
        *screen_stack + OsuOffsets::ScreenStack_stack);

    auto count = Memory::ReadAs<uint32_t>(handle_, *stack + 0x10);

    auto items = Memory::ReadAs<uintptr_t>(handle_, *stack + 0x8);


    if (count <= 0 || items == 0 || !screen_stack || !stack || !count || !items) {
        return std::nullopt;
    }

    return Memory::ReadAs<uintptr_t>(handle_, *items + 0x10 + 0x8 * (*count - 1));
}

std::optional<uintptr_t> OsuInfoProvider::GetScoreInfo()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return std::nullopt;
    }

    auto player_score = Memory::ReadAs<uintptr_t>(handle_, screen.value() + OsuOffsets::Player_Score);
    if (!player_score) {
        return std::nullopt;
    }

    auto score_info = Memory::ReadAs<uintptr_t>(handle_, *player_score + 0x8);
    if (!score_info) {
        return std::nullopt;
    }

	return score_info == 0 ? std::nullopt : std::optional<uintptr_t>(score_info);
}

OsuInfoProvider::OsuInfoProvider(HandleRaii handle, const uintptr_t& base_address) :
    base_address_(base_address), handle_(std::move(handle))
{}

std::vector<std::string> OsuInfoProvider::GetCurrentMods()
{

    std::vector<std::string> mods;
    auto score_info = GetScoreInfo();
    if (!score_info || !GetPlayingState())
        return mods;

    auto string_address = Memory::ReadAs<uintptr_t>(handle_, score_info.value() + OsuOffsets::ScoreInfo_ModsJson);
    if (!string_address)
        return mods;

    std::wstring mods_json = DotNetString::Read(handle_, *string_address).value_or(L"");

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

int OsuInfoProvider::GetCurrentCombo()
{
    if (!GetPlayingState()) return 1;

    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return -1;
    }

    auto score = Memory::ReadAs<uintptr_t>(handle_,
        screen.value() + OsuOffsets::Player_ScoreProcessor);

    if (!score) {
        return -1;
    }

    auto combo = Memory::ReadAs<uintptr_t>(handle_,
        *score + OsuOffsets::OsuScoreProcessor_Combo);
    if (!combo) {
        return -1;
    }

    auto combo_value = Memory::ReadAs<int>(handle_, combo.value() + 0x40);
    if (!combo_value) {
        return -1;
    }

    return *combo_value;
}

bool OsuInfoProvider::GetPlayingState()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return false;
    }

    auto game_api = Memory::ReadAs<uintptr_t>(handle_,
        base_address_ + OsuOffsets::OsuGameBase_API);
    auto game_score_manager = Memory::ReadAs<uintptr_t>(handle_,
        base_address_ + OsuOffsets::OsuGameBase_ScoreManager);
    auto player_api = Memory::ReadAs<uintptr_t>(handle_,
        screen.value() + OsuOffsets::Player_api);
    auto player_score_manager = Memory::ReadAs<uintptr_t>(handle_,
        screen.value() + OsuOffsets::Player_scoreManager);

    if (!game_api || !game_score_manager || !player_api || !player_score_manager) {
        return false;
    }

    return player_api == game_api && player_score_manager == game_score_manager;
}
