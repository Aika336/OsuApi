#pragma once

#include <vector>
#include "Process_info.h"

class ProvideProcesses {
	ProcessInfo FindProcessByName(const std::string& name);
public:
	static std::vector<ProcessInfo> GetAllProcesses();
	static ProcessInfo GetProcessByName(const std::string& name);
};