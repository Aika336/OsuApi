#pragma once

#include "HandlerRaii.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <optional>

class ProvideOsuInfo {
	uintptr_t base_address_;
	Handler_raii handler_;

	std::optional<uintptr_t> GetCurrentScreenAddress();
	std::optional<uintptr_t> GetScoreInfo();
public:
	ProvideOsuInfo(Handler_raii handler, const uintptr_t& base_address);
	std::vector<std::string> GetCurrentMods();
	int GetCurrentCombo();
	bool GetPlayingState();
};