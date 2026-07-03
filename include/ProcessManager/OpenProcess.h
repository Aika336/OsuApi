#pragma once

#include <Windows.h>
#include "HandlerRaii.h"
#include <Process_info.h>

class OpenProcess {
public:
	static std::shared_ptr<Handler_raii> OpenProcessByIdW(ProcessInfo processId);
	static std::shared_ptr<Handler_raii> OpenProcessByIdR(ProcessInfo processId);
	static std::shared_ptr<Handler_raii> OpenProcessByIdRW(ProcessInfo processId);
};