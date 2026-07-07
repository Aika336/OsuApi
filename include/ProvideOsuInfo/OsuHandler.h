#pragma once

#include "ProviderOsuInfo.h"
#include "OsuSignatureInfo.h"

#include <vector>
#include <string>
#include <memory>

struct GameState {
	bool is_playing = false;
	int current_combo = -1;
	std::vector<std::string> current_mods;
};

class OsuHandler {
	GameState game_state_;
    std::unique_ptr<OsuInfoProvider> osu_info_;
public:
	OsuHandler();
	void UpdateGameState();
	const GameState GetGameState();
};