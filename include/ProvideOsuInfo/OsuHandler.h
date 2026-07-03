#pragma once
#include <vector>
#include <string>
#include <ProviderOsuInfo.h>

struct GameState {
	bool is_playing = false;
	int current_combo = -1;
	std::vector<std::string> current_mods;
};

class OsuHandler {
	GameState game_state_;
	ProvideOsuInfo* osu_info_;
public:
	OsuHandler();
	const void UpdateGameState();
	const GameState& GetGameState();
};