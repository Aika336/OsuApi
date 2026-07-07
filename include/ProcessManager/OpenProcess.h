#pragma once

#include "HandlerRaii.h"
#include <Process_info.h>

#include <Windows.h>

class OpenProcess {
public:
	static Handler_raii OpenProcessByIdW(ProcessInfo processId);
	static Handler_raii OpenProcessByIdR(ProcessInfo processId);
	static Handler_raii OpenProcessByIdRW(ProcessInfo processId);
};