#include "OsuHandler.h"
#include <OpenProcess.h>
#include <ProvideProcesses.h>
#include <ProcessMemoryScanner.h>
#include "Logger.h"

OsuHandler::OsuHandler()
{
	auto process_info = ProcessProvider::GetProcessByName(L"osu!.exe");
	if (!process_info) {
		LOG_ERROR("Can not find a process with name osu!.exe");
	}

	HandleRaii handler = ProcessOpener::OpenProcessForReadWrite(process_info.value());

	auto game_base_address = ProcessMemoryScanner::ScanProcessMemoryForSignature(handler, PatternMatcher(signature_, mask_, 32));
	if (!game_base_address) {
		LOG_ERROR("Failed to find the game base address.");
	}

	osu_info_ = std::make_unique<OsuInfoProvider>(std::move(handler), game_base_address.value());
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
