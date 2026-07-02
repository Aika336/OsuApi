#pragma once

#include "ProvideProcesses.h"

#include <TlHelp32.h>
#include <iostream>

ProcessInfo ProvideProcesses::FindProcessByName(const std::wstring& name)
{
	std::vector<ProcessInfo> processes = GetAllProcesses();
	if (processes.size() == 0) {
		std::cout << "No processes found." << std::endl;
		return ProcessInfo();
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