#pragma once
#include <vector>
#include <string>
#include <ProviderOsuInfo.h>

const std::vector<uint8_t> signature_ = {
        0x67, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x6E, 0x00,
        0x20, 0x00, 0x34, 0x00, 0x35, 0x00, 0x33, 0x00,
        0x35, 0x00, 0x37, 0x00, 0x29, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
};

const std::vector<uint8_t> mask_ = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

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
	void UpdateGameState();
	const GameState GetGameState();
    ~OsuHandler() { delete osu_info_; }
};