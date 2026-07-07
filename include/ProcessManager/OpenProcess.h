#pragma once

#include "HandlerRaii.h"
#include <Process_info.h>

#include <Windows.h>

class OpenProcess {
public:
	static HandleRaii OpenProcessByIdW(ProcessInfo processId);
	static HandleRaii OpenProcessByIdR(ProcessInfo processId);
	static HandleRaii OpenProcessByIdRW(ProcessInfo processId);
};