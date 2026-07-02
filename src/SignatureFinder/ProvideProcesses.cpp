#pragma once

#include "ProvideProcesses.h"

#include <TlHelp32.h>
#include <iostream>

std::vector<ProcessInfo> ProvideProcesses::GetAllProcesses() {
	std::vector<ProcessInfo> processes;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create process snapshot." << std::endl;
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