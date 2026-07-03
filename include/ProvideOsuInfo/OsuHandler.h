#pragma once
#include <vector>
#include <string>

struct GameState {
	bool is_playing = false;
	int current_combo = -1;
	std::vector<std::string> current_mods;
};

class OsuHandler {

public:

};