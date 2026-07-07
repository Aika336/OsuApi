#pragma once

#include "PatternMatcher.h"
#include "../ProcessManager/HandlerRaii.h"

#include <optional>

class ProcessMemoryScanner {
public:
	static std::optional<uintptr_t> ScanProcessMemoryForSignature(const Handler_raii& handler, PatternMatcher mather);
};