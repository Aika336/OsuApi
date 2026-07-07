#pragma once

#include "../ProcessManager/HandlerRaii.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

class Memory {
public:
	template<typename T>
	static std::optional<T> ReadAs(const HandleRaii& handler, uintptr_t address);

	static std::optional<std::vector<uint8_t>> ReadMemoryRegion(
		const HandleRaii& handler, 
		uintptr_t address, size_t size
	);
};

template<typename T>
inline std::optional<T> Memory::ReadAs(const HandleRaii& handler, uintptr_t address)
{
	T value{};
	SIZE_T read_bytes = 0;

	if (!ReadProcessMemory(handler.hProcess, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &read_bytes) 
		|| read_bytes != sizeof(T)) {
		std::cout << "Failed to read memory at address: " << std::hex << address << std::dec << std::endl;
		return std::nullopt;
	}

	return value;
}