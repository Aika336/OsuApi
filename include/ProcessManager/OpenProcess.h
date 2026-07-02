#pragma once

#include <Windows.h>
#include "HandlerRaii.h"

class OpenProcess {
public:
	static Handler_raii OpenProcessByIdW(DWORD processId);
	static Handler_raii OpenProcessByIdR(DWORD processId);
	static Handler_raii OpenProcessByIdRW(DWORD processId);
};