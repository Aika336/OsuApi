#pragma once

#include <vector>
#include <string>

class PatternMatcher {
	std::vector<uint8_t> signature;
	std::vector<uint8_t> mask;
public:
	PatternMatcher() = delete;
	PatternMatcher(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& mask) : signature(signature), mask(mask) {}
	int CheckForStableSignature(const std::vector<uint8_t>& region_bytes) const;
};