#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstdint>

struct GameState {
	// Base information
	bool is_playing = false;
	int combo = 0;
	int16_t game_mode = 0;

	// Mods
	std::vector<std::string> enable_mods;
	std::string mods = "NM";

	// Memory addresses
	uintptr_t osu_base_address = 0;
	uintptr_t screen_address = 0;

	bool validation = true;
	std::string last_error = "";
};

struct ReadResult {
	bool success = false;
	std::string error;
	GameState game_state;
};