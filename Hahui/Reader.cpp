//
// Created by Aika on 28.06.2026.
//
#pragma once
#include <algorithm>
#include <stdexcept>
#include "Memory.cpp"
#include "Reader.h"
#include "Offsets.h"
#include "ProcessUtils.cpp"

constexpr size_t value_offset_ = 32;
static uint8_t NONCONST_BYTE_ = 0x02;
static int STABLE_AVAILABLE_MISSES_ = 0;

OsuReader::OsuReader() {
    process_handle_ = ProcessUtils::OpenProcessByName(L"osu!.exe");
    FindGameBaseAddress();
}

OsuReader::~OsuReader() = default;

inline int OsuReader::CheckForStableSignature(const std::vector<uint8_t>& bytes_region)
{
    const size_t region_size = bytes_region.size();
    const size_t sig_size = OsuOffsets::signature_.size();

    if (sig_size == 0 || region_size < sig_size) return -1;

    const uint8_t* data = bytes_region.data();
    const uint8_t* sig = OsuOffsets::signature_.data();

    // Предварительный подсчет константных байтов в сигнатуре
    size_t const_count = 0;
    for (size_t i = 0; i < sig_size; ++i) {
        if (sig[i] != NONCONST_BYTE_) const_count++;
    }

    // Если все байты wildcard
    if (const_count == 0) {
        return static_cast<int>(value_offset_);
    }

    // Минимальное количество совпадений, необходимое для успеха
    const size_t min_matches = const_count - STABLE_AVAILABLE_MISSES_;

    const size_t max_pos = region_size - sig_size;

    for (size_t pos = 0; pos <= max_pos; ++pos) {
        // Быстрое сравнение с подсчетом несовпадений
        size_t mismatches = 0;
        bool found = true;

        for (size_t i = 0; i < sig_size; ++i) {
            if (sig[i] != NONCONST_BYTE_ && data[pos + i] != sig[i]) {
                mismatches++;
                if (mismatches > STABLE_AVAILABLE_MISSES_) {
                    found = false;
                    break;
                }
            }
        }

        if (found) {
            // std::cout << "[DBG] Signature found at chunk pos " << pos << std::endl;
            return static_cast<int>(pos + value_offset_);
        }
    }

    return -1;
}

inline GameState& OsuReader::GetLastGameState() {
    return game_state_;
}

inline int32_t OsuReader::ReadGameMode() {
    uintptr_t ruleset_bindable = Memory::RPM<uintptr_t>(process_handle_, game_address_ + OsuOffsets::OsuGameDesktop_Ruleset);
    uintptr_t ruleset_info = Memory::RPM<uintptr_t>(process_handle_, ruleset_bindable + 0x20);
    return Memory::RPM<int32_t>(process_handle_, ruleset_info + OsuOffsets::RulesetInfo_OnlineID);
}

inline std::vector<std::string> OsuReader::ReadEnabledMods(uintptr_t score_info) {
    std::vector<std::string> mods;
    if (score_info == 0) return mods;

    uintptr_t string_address = Memory::RPM<uintptr_t>(process_handle_, score_info + OsuOffsets::ScoreInfo_ModsJson);
    std::wstring mods_json = Memory::ReadDotNetString(process_handle_, string_address);

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

inline int32_t OsuReader::ReadCombo(uintptr_t current_screen) {
    uintptr_t score = Memory::RPM<uintptr_t>(process_handle_,
        current_screen + OsuOffsets::Player_ScoreProcessor);

    if (score == 0) return -1;

    uintptr_t combo = Memory::RPM<uintptr_t>(process_handle_,
        score + OsuOffsets::OsuScoreProcessor_Combo);
    if (combo == 0) return -1;

    return Memory::RPM<int32_t>(process_handle_, combo + 0x40);
}

inline uintptr_t OsuReader::FindCurrentScreen() {
    uintptr_t screen_stack = Memory::RPM<uintptr_t>(process_handle_,
        game_address_ + OsuOffsets::OsuGame_ScreenStack);

    uintptr_t stack = Memory::RPM<uintptr_t>(process_handle_,
        screen_stack + OsuOffsets::ScreenStack_stack);

    uint32_t count = Memory::RPM<uint32_t>(process_handle_, stack + 0x10);

    uintptr_t items = Memory::RPM<uintptr_t>(process_handle_, stack + 0x8);

    if (count <= 0 || items == 0)
        return 0;

    return Memory::RPM<uintptr_t>(process_handle_, items + 0x10 + 0x8 * (count - 1));
}

inline bool OsuReader::ValidatePlayerScreen(uintptr_t current_screen) {
    uintptr_t game_api = Memory::RPM<uintptr_t>(process_handle_,
        game_address_ + OsuOffsets::OsuGameBase_API);
    uintptr_t game_score_manager = Memory::RPM<uintptr_t>(process_handle_,
        game_address_ + OsuOffsets::OsuGameBase_ScoreManager);
    uintptr_t player_api = Memory::RPM<uintptr_t>(process_handle_,
        current_screen + OsuOffsets::Player_api);
    uintptr_t player_score_manager = Memory::RPM<uintptr_t>(process_handle_,
        current_screen + OsuOffsets::Player_scoreManager);

    if (game_api == 0 || game_score_manager == 0 || player_api == 0 || player_score_manager == 0) {
        return false;
    }

    return player_api == game_api && player_score_manager == game_score_manager;
}

inline bool OsuReader::FindGameBaseAddress() {
    SIZE_T scan_addr = 0;
    MEMORY_BASIC_INFORMATION mbi;
    while (VirtualQueryEx(process_handle_, reinterpret_cast<LPVOID>(scan_addr), &mbi, sizeof(mbi))) {
        SIZE_T region_end = (SIZE_T)mbi.BaseAddress + (SIZE_T)mbi.RegionSize;

        if (mbi.State == MEM_COMMIT &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)) {

            std::vector<uint8_t> regionBytes(mbi.RegionSize);
            SIZE_T bytes_read = 0;
            if (ReadProcessMemory(process_handle_, mbi.BaseAddress, regionBytes.data(), mbi.RegionSize, &bytes_read) > 0) {
                int offset = OsuReader::CheckForStableSignature(regionBytes);
                if (offset >= 0) {
                    game_address_ = static_cast<uintptr_t>((SIZE_T)mbi.BaseAddress + offset);

                    return true;
                }
            }
        }

        scan_addr = region_end;
    }
    return false;
}

inline ReadResult OsuReader::ReadGameState() {
    ReadResult result;

    game_state_.game_mode = ReadGameMode();

    // Поиск текущего экрана
    uintptr_t current_screen = FindCurrentScreen();
    if (!current_screen) {
        result.success = false;
        result.error = "Failed to find current screen";
        result.game_state = game_state_;
        return result;
    }

    game_state_.screen_address = current_screen;

    // Проверка, что это экран Player
    if (!ValidatePlayerScreen(current_screen)) {
        game_state_.is_playing = false;
        result.success = true;
        result.game_state = game_state_;
        return result;
    }

    game_state_.is_playing = true;

    // Чтение информации оScore
    uintptr_t player_score = Memory::RPM<uintptr_t>(process_handle_, current_screen + OsuOffsets::Player_Score);
    uintptr_t score_info = Memory::RPM<uintptr_t>(process_handle_, player_score + 0x8);

    // Чтение модификаторов
    std::vector<std::string> enabled_mods = ReadEnabledMods(score_info);
    game_state_.enable_mods = enabled_mods;

    // Объединение модов в строку
    if (enabled_mods.empty()) {
        game_state_.mods = "NM";
    }
    else {
        std::string mods_str;
        for (const auto& mod : enabled_mods) {
            mods_str += mod;
        }
        game_state_.mods = mods_str;
    }

    // Чтение комбо
    int32_t combo = ReadCombo(current_screen);
    game_state_.combo = combo;

    result.success = true;
    result.game_state = game_state_;
    return result;
}
