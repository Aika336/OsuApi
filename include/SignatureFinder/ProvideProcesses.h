#pragma once

#include <vector>
#include "Process_info.h"

class ProvideProcesses {
	std::vector<ProcessInfo> processes;
	
	ProcessInfo FindProcessByName(const std::string& name);
public:
	std::vector<ProcessInfo> GetAllProcesses();
	ProcessInfo GetProcessByName(const std::string& name);
};