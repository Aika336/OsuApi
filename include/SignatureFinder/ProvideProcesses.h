#pragma once

#include <vector>
#include "Process_info.h"

class ProvideProcesses {
	static ProcessInfo FindProcessByName(const std::wstring& name);
public:
	static std::vector<ProcessInfo> GetAllProcesses();
	static ProcessInfo GetProcessByName(const std::wstring& name);
};