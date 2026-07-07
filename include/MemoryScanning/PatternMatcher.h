#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <optional>

class PatternMatcher {
	std::vector<uint8_t> signature_;
	std::vector<uint8_t> signature_mask_;
	int offset;
public:
	PatternMatcher() = delete;
	PatternMatcher(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& mask, int offset);
	std::optional<size_t> CheckForSignature(const std::vector<uint8_t>& region_bytes) const;
	int getOffset() const;
};