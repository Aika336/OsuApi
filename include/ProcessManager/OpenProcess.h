#pragma once

#include <Windows.h>
#include "HandlerRaii.h"
#include <Process_info.h>

class OpenProcess {
public:
	static Handler_raii OpenProcessByIdW(ProcessInfo processId);
	static Handler_raii OpenProcessByIdR(ProcessInfo processId);
	static Handler_raii OpenProcessByIdRW(ProcessInfo processId);
};