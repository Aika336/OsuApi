#pragma once

#include "ProvideProcesses.h"

#include <TlHelp32.h>
#include <iostream>

ProcessInfo ProvideProcesses::FindProcessByName(const std::wstring& name)
{
	std::vector<ProcessInfo> processes = GetAllProcesses();
	if (processes.size() == 0) {
		std::cout << "ProvideProcesses::FindProcessByName: No processes found." << std::endl;
		return ProcessInfo(0,L"");
	}

	for (const auto& process : processes) {
		if (process.name == name) {
			return process;
		}
	}

	return ProcessInfo();
}

std::vector<ProcessInfo> ProvideProcesses::GetAllProcesses() {
	std::vector<ProcessInfo> processes;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "ProvideProcesses::GetAllProcesses: Failed to create process snapshot." << std::endl;
		return processes;
	}

	PROCESSENTRY32W pe = { sizeof(pe) };
	if (Process32FirstW(snapshot, &pe)) {
		do {
			ProcessInfo p{ pe.th32ProcessID, std::wstring(pe.szExeFile) };
			processes.push_back(p);
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return processes;
}

ProcessInfo ProvideProcesses::GetProcessByName(const std::wstring& name)
{
	ProcessInfo process = FindProcessByName(name);
	if (process.processId == 0) {
		return ProcessInfo(0, L"");
	}

	return process;
}
