#pragma once
#include <Windows.h>
#include "HandlerRaii.h"
#include <vector>
#include <string>
#include <optional>

class ProvideOsuInfo {
	uintptr_t base_address_;
	uintptr_t current_screen;
	Handler_raii handler_;

	std::optional<uintptr_t> GetCurrentScreenAddress();
	std::optional<uintptr_t> GetScoreInfo();
public:
	ProvideOsuInfo(Handler_raii handler, const uintptr_t &base_address) : base_address_(base_address), handler_(std::move(handler)){}
	std::vector<std::string> GetCurrentMods();
	int GetCurrentCombo();
	bool GetPlayingState();
	~ProvideOsuInfo() {
		CloseHandle(handler_.hProcess);
	}
};