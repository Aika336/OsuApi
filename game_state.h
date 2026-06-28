#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstdint>

struct GameState {
	// Base information
	bool is_playing_ = false;
	int combo_ = 0;
	int16_t game_mode_ = 0;

	// Mods
	std::vector<std::string> enable_mods_;
	std::string mods_ = "NM";

	// Memory addresses
	uintptr_t osu_base_address_ = 0;
	uintptr_t screen_address_ = 0;

	bool validation_ = true;
	std::string last_error_ = "";
};

struct ReadResult {
	bool success_ = false;
	std::string error_;
	GameState game_state_;
};