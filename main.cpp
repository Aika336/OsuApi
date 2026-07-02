#include <iostream>
#include <OpenProcess.h>
#include <ProvideProcesses.h>
#include <ProcessMemoryScanner.h>

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

int main() {

	auto process_info = ProvideProcesses::GetProcessByName(L"osu!.exe");
	if (!process_info) {
		std::cout << "Process not found." << std::endl;
		return 1;
	}

	Handler_raii handler = OpenProcess::OpenProcessByIdRW(process_info.value());

    auto game_base_address = ProcessMemoryScanner::ScanProcessMemoryForSignature(handler, PatternMatcher(signature_, mask_, 32));
    if (!game_base_address) {
		std::cout << "Failed to find the game base address." << std::endl;
        return 1;
    }

	std::cout << std::hex << game_base_address.value() << std::dec << std::endl;
    int al;
    std::cin >> al;

    return 0;
}