#pragma once

#include "ProcessInfo.h"

#include <vector>
#include <optional>

class ProcessProvider {
	static std::optional<ProcessInfo> FindProcessByName(const std::wstring& name);
public:
	static std::vector<ProcessInfo> GetAllProcesses();
	static std::optional<ProcessInfo> GetProcessByName(const std::wstring& name);
};