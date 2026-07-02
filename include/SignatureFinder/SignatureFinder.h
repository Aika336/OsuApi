#pragma once

#include "HandlerRaii.h"

#include <Windows.h>
#include <vector>

class SignatureFinder {
	size_t value_offset;
	uint8_t NONCONST_BYTE;
	std::vector<uint8_t> signature;
	Handler_raii handler;

	int CheckForStableSignature(const std::vector<uint8_t>& bytes_region);
public:
	SignatureFinder(Handler_raii handler, const std::vector<uint8_t>& signature_, uint8_t NONCONST_BYTE_, size_t value_offset_)
		: value_offset(value_offset_), NONCONST_BYTE(NONCONST_BYTE_), signature(signature_), handler(std::move(handler)) {}

	uintptr_t FindAddressBySignature();
	uintptr_t FindAddressBySignature(MEMORY_BASIC_INFORMATION filter);
};