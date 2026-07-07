#pragma once

#include "PatternMatcher.h"
#include "../ProcessManager/HandleRaii.h"

#include <optional>

class ProcessMemoryScanner {
public:
	static std::optional<uintptr_t> ScanProcessMemoryForSignature(const HandleRaii& handler, PatternMatcher mather);
};