#pragma once

#include "Memory.h"
#include <Windows.h>
#include <vector>

std::optional<std::vector<uint8_t>> Memory::ReadMemoryRegion(const Handler_raii& handler, uintptr_t address, size_t size) {
	std::vector<uint8_t> buffer(size);
	SIZE_T read_bytes = 0;
	if (!ReadProcessMemory(handler.hProcess, reinterpret_cast<LPVOID>(address), buffer.data(), size, &read_bytes)) {
		std::cout << "Failed to read memory region at address: " << std::hex << address << std::dec << std::endl;
		return std::nullopt;
	}

	return buffer;
}