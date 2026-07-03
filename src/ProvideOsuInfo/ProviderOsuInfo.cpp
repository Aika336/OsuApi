#include "ProviderOsuInfo.h"
#include <OsuOffset.h>
#include "../../include/Memory/Memory.h"
#include <DotNetString.h>

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

std::optional<uintptr_t> ProvideOsuInfo::GetScoreInfo()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return std::nullopt;
    }

    uintptr_t player_score = Memory::RPM<uintptr_t>(handler_, screen.value() + OsuOffsets::Player_Score).value();
    uintptr_t score_info = Memory::RPM<uintptr_t>(handler_, player_score + 0x8).value();

	return score_info == 0 ? std::nullopt : std::optional<uintptr_t>(score_info);
}

std::optional<std::vector<std::string>> ProvideOsuInfo::GetCurrentMods()
{
    auto score_info = ProvideOsuInfo::GetScoreInfo();
    if (score_info == std::nullopt)
        return std::nullopt;

    std::vector<std::string> mods;

    uintptr_t string_address = Memory::RPM<uintptr_t>(handler_, score_info.value() + OsuOffsets::ScoreInfo_ModsJson).value();
    std::wstring mods_json = DotNetString::Read(handler_, string_address).value_or(L"");

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

std::optional<uint32_t> ProvideOsuInfo::GetCurrentCombo()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
        return std::nullopt;
    }

    uintptr_t score = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_ScoreProcessor).value();

    if (score == 0) return std::nullopt;

    uintptr_t combo = Memory::RPM<uintptr_t>(handler_,
        score + OsuOffsets::OsuScoreProcessor_Combo).value();
    if (combo == 0) return std::nullopt;

    return Memory::RPM<int32_t>(handler_, combo + 0x40);
}

std::optional<bool> ProvideOsuInfo::GetPlayingState()
{
    auto screen = GetCurrentScreenAddress();
    if (!screen) {
        std::cout << "GetPlayingState: can't get get current screen address" << std::endl;
		return std::nullopt;
    }

    uintptr_t game_api = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_API).value();
    uintptr_t game_score_manager = Memory::RPM<uintptr_t>(handler_,
        base_address_ + OsuOffsets::OsuGameBase_ScoreManager).value();
    uintptr_t player_api = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_api).value();
    uintptr_t player_score_manager = Memory::RPM<uintptr_t>(handler_,
        screen.value() + OsuOffsets::Player_scoreManager).value();

    if (game_api == 0 || game_score_manager == 0 || player_api == 0 || player_score_manager == 0) {
        return std::nullopt;
    }

    return player_api == game_api && player_score_manager == game_score_manager;
}
