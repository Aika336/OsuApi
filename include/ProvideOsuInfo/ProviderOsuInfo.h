#pragma once
#include <Windows.h>
#include <optional>
#include "HandlerRaii.h"

class ProvideOsuInfo {
	uintptr_t base_address_;
	uintptr_t current_screen;
	const Handler_raii& handler_;

	std::optional<uintptr_t> GetCurrentScreenAddress();
	std::optional<uintptr_t> GetScoreInfo();
public:
	ProvideOsuInfo(const Handler_raii& handler, const uintptr_t &base_address) : base_address_(base_address), handler_(handler){}
	std::vector<std::string> GetCurrentMods();
	std::optional<uint32_t> GetCurrentCombo();
	std::optional<bool> GetPlayingState();
};