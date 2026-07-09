#pragma once

#include "HandleRaii.h"
#include "ProcessInfo.h"

class ProcessOpener {
public:
	static HandleRaii OpenProcessForWrite(ProcessInfo processId);
	static HandleRaii OpenProcessForRead(ProcessInfo processId);
	static HandleRaii OpenProcessForReadWrite(ProcessInfo processId);
};