#pragma once

#include "HandlerRaii.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <optional>

class OsuInfoProvider {
	uintptr_t base_address_;
	HandleRaii handler_;

	std::optional<uintptr_t> GetCurrentScreenAddress();
	std::optional<uintptr_t> GetScoreInfo();
public:
	OsuInfoProvider(HandleRaii handler, const uintptr_t& base_address);
	std::vector<std::string> GetCurrentMods();
	int GetCurrentCombo();
	bool GetPlayingState();
};