//
// Created by Aika on 28.06.2026.
//
#pragma once

#include "game_state.h"
#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

class OsuReader {
    HANDLE process_handle_;
    GameState game_state_;
    uintptr_t game_address_ = 0;

    int32_t ReadGameMode();
    std::vector<std::string> ReadEnabledMods(uintptr_t score_info);
    int32_t ReadCombo(uintptr_t current_screen);
    uintptr_t FindCurrentScreen();
    bool ValidatePlayerScreen(uintptr_t current_screen);

    bool FindGameBaseAddress();
    int CheckForStableSignature(const std::vector<uint8_t>& bytes_region);

public:
    OsuReader();

    ~OsuReader();

    ReadResult ReadGameState();
    const GameState& GetLastGameState();
};
