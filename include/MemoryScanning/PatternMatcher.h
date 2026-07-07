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
	PatternMatcher(const std::vector<uint8_t>& signature, const std::vector<uint8_t>& mask, int offset) : signature_(signature), signature_mask_(mask), offset(offset) {
		if (signature.size() != mask.size()) {
			throw std::runtime_error("Error: Signature and mask must be of the same size.");
		}
	}
	std::optional<size_t> CheckForSignature(const std::vector<uint8_t>& region_bytes) const;
	int getOffset() const { return offset; }
};