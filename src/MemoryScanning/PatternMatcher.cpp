#include "PatternMatcher.h"

std::optional<size_t> PatternMatcher::CheckForStableSignature(const std::vector<uint8_t>& region_bytes) const
{
	const size_t sig_size = signature_.size();

	if (sig_size == 0 || sig_size != signature_mask_.size() || sig_size > region_bytes.size()) {
		return std::nullopt;
	}

	const size_t last_index = region_bytes.size() - sig_size;

	for (size_t offset = 0; offset <= last_index; offset++) {
		bool match = true;
		for (size_t i = 0; i < sig_size; i++) {
			if (signature_mask_[i] && region_bytes[offset + i] != signature_[i]) {
				match = false;
				break;
			}
		}

		if (match) {
			return static_cast<int>(offset);
		}
	}

    return std::nullopt;
}
