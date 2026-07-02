#pragma once

#include "../SignatureFinder/HandlerRaii.h"
#include <Windows.h>
#include <string>
#include <vector>

class Memory {
public:
	template<typename T>
	static T RPM(Handler_raii handler, uintptr_t address);

	static std::wstring ReadDotNetString(Handler_raii handler, uintptr_t string_address);
	static std::vector<uint8_t> ReadMemoryRegion(Handler_raii handler, uintptr_t address, size_t size);
};

template<typename T>
inline T Memory::RPM(Handler_raii handler, uintptr_t address)
{
	T value{};
	SIZE_T read_bytes = 0;

	if (!ReadProcessMemory(handler.hProcess, static_cast<LPVOID>(address), &value, sizeof(T), &read_bytes) 
		|| read_bytes != sizeof(T)) {
		std::cout << "Failed to read memory at address: " << std::hex << address << std::dec << std::endl;
		return T{};
	}

	return value;
}

inline std::vector<uint8_t> Memory::ReadMemoryRegion(Handler_raii handler, uintptr_t address, size_t size) {
	std::vector<uint8_t> buffer(size);
	SIZE_T read_bytes = 0;
	if (!ReadProcessMemory(handler.hProcess, reinterpret_cast<LPVOID>(address), buffer.data(), size, &read_bytes)) {
		buffer.clear();
	}

	return buffer;
}