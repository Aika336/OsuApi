#pragma once

#include <vector>
#include "Process_info.h"
#include <optional>

class ProvideProcesses {
	static std::optional<ProcessInfo> FindProcessByName(const std::wstring& name);
public:
	static std::vector<ProcessInfo> GetAllProcesses();
	static std::optional<ProcessInfo> GetProcessByName(const std::wstring& name);
};