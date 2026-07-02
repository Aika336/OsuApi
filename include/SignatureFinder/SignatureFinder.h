#pragma once

#include <Windows.h>
#include <vector>

class SignatureFinder {
public:
	static uintptr_t FindAddressBySignature(const std::vector<uint8_t>& signature);
	static uintptr_t FindAddressBySignature(const std::vector<uint8_t>& signature, MEMORY_BASIC_INFORMATION filter);
};