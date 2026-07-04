#include "OsuHandler.h"
#include <OpenProcess.h>
#include <ProvideProcesses.h>
#include <ProcessMemoryScanner.h>

OsuHandler::OsuHandler()
{
	auto process_info = ProvideProcesses::GetProcessByName(L"osu!.exe");
	if (!process_info) {
		std::cout << "Process not found." << std::endl;
	}

	Handler_raii handler = OpenProcess::OpenProcessByIdRW(process_info.value());

	auto game_base_address = ProcessMemoryScanner::ScanProcessMemoryForSignature(handler, PatternMatcher(signature_, mask_, 32));
	if (!game_base_address) {
		std::cout << "Failed to find the game base address." << std::endl;
	}

	osu_info_ = std::make_unique<ProvideOsuInfo>(std::move(handler), game_base_address.value());
}

void OsuHandler::UpdateGameState()
{
	game_state_.current_combo = osu_info_->GetCurrentCombo();
	game_state_.current_mods = osu_info_->GetCurrentMods();
	game_state_.is_playing = osu_info_->GetPlayingState();
}

const GameState OsuHandler::GetGameState()
{
	return game_state_;
}
