#pragma once

#include "HandlerRaii.h"
#include <ProcessInfo.h>

#include <Windows.h>

class OpenProcess {
public:
	static HandleRaii OpenProcessForWrite(ProcessInfo processId);
	static HandleRaii OpenProcessForRead(ProcessInfo processId);
	static HandleRaii OpenProcessForReadWrite(ProcessInfo processId);
};