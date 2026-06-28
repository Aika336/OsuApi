#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

class Memory {
public:
	// Open process using his pId
	static HANDLE OpenProcess(DWORD pId);

	// Close process
	static void CloseProcess(HANDLE hProcess);

	// Read memory by address
	template <typename T>
	static T RPM(HANDLE hProcess, uintptr_t address);

	// Read .net string
	static std::wstring ReadDotNetString(HANDLE hProcess, uintptr_t string_address);

	// Read memory address by address and how many bytes read
	static std::vector<uint8_t> ReadMemoryRegion(HANDLE hProcess, uintptr_t address, size_t size);
};