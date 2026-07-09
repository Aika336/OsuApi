#pragma once

#include "../ProcessManager/HandleRaii.h"
#include "Logger.h"
#include "ntdll.h"

#include <vector>
#include <iostream>
#include <optional>
#include <format>

class Memory {
public:
	template<typename T>
	static std::optional<T> ReadAs(const HandleRaii& handler, uintptr_t address);

	static std::optional<std::vector<uint8_t>> ReadMemoryRegion(
		const HandleRaii& handler, 
		uintptr_t address, size_t size
	);
	static NtDll ntdll_;
};

template<typename T>
inline std::optional<T> Memory::ReadAs(const HandleRaii& handler, uintptr_t address)
{
	T value{};
	SIZE_T read_bytes = 0;

	NTSTATUS status = ntdll_.NtReadProcessMemory(
		handler.hProcess,
		reinterpret_cast<LPVOID>(address),
		&value,
		sizeof(T),
		&read_bytes
	);

	if (!NT_SUCCESS(status)) {
		LOG_ERROR("Failed to read memory at address: " + std::format("0x{:x}", address));
		return std::nullopt;
	}
	else if (sizeof(T) != read_bytes) {
		LOG_ERROR("Read bytes != expected bytes: " + std::format("0x{:x}", address));
		return std::nullopt;
	}

	return value;
}