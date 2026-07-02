#pragma once

#include <vector>
#include <string>
#include <iostream>

class PatternMatcher {
	std::vector<uint8_t> signature;
	std::vector<uint8_t> mask;
	int offset;
public:
	PatternMatcher() = delete;
	PatternMatcher(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& mask, int offset) : signature(signature), mask(mask), offset(offset) {
		if (signature.size() != mask.size()) {
			std::cerr << "Error: Signature and mask must be of the same size." << std::endl;
		}
	}
	int CheckForStableSignature(const std::vector<uint8_t>& region_bytes) const;
	int getOffset() const { return offset; }
};