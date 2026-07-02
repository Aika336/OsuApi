#pragma once
#include "PatternMatcher.h"
#include "../ProcessManager/HandlerRaii.h"

class ProcessMemoryScanner {
public:
	static uintptr_t ScanProcessMemoryForSignature(const Handler_raii& handler, PatternMatcher mather);
};