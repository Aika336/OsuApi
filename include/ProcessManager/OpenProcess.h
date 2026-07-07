#pragma once

#include "HandlerRaii.h"
#include <Process_info.h>

#include <Windows.h>

class OpenProcess {
public:
	static HandleRaii OpenProcessForWrite(ProcessInfo processId);
	static HandleRaii OpenProcessForRead(ProcessInfo processId);
	static HandleRaii OpenProcessForReadWrite(ProcessInfo processId);
};