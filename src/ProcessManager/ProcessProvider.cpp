#include "ProcessProvider.h"
#include <Logger.h>

#include <TlHelp32.h>
#include <iostream>

std::optional<ProcessInfo> ProcessProvider::FindProcessByName(const std::wstring& name)
{
	std::vector<ProcessInfo> processes = GetAllProcesses();
	if (processes.size() == 0) {
		throw std::runtime_error("No process found");
	}

	for (const auto& process : processes) {
		if (process.name == name) {
			return process;
		}
	}

	throw std::runtime_error("ProvideProcesses::FindProcessByName: No find process by name " + std::string(name.begin(), name.end()));
}

std::vector<ProcessInfo> ProcessProvider::GetAllProcesses() {
	std::vector<ProcessInfo> processes;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("ProvideProcesses::GetAllProcesses: Failed to create process snapshot.");
	}

	PROCESSENTRY32W pe = { sizeof(pe) };
	if (Process32FirstW(snapshot, &pe)) {
		do {
			ProcessInfo p{ pe.th32ProcessID, std::wstring(pe.szExeFile) };
			processes.emplace_back(p);
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return processes;
}

std::optional<ProcessInfo> ProcessProvider::GetProcessByName(const std::wstring& name)
{
	return FindProcessByName(name);
}