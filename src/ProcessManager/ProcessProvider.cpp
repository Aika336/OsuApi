#include "ProcessProvider.h"
#include "Logger.h"
#include "Memory.h"

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

/*std::vector<ProcessInfo> ProcessProvider::GetAllProcesses() {
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
}*/

std::vector<ProcessInfo> ProcessProvider::GetAllProcesses() {
	std::vector<ProcessInfo> processes;
	
	ULONG systeminfo_len = 0;
	NTSTATUS status = Memory::ntdll_.NtQuerySystemInformation(
		SystemProcessInformation,
		nullptr,
		systeminfo_len,
		&systeminfo_len
	);

	const NTSTATUS STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;
	if (status != STATUS_INFO_LENGTH_MISMATCH) {
		LOG_ERROR("Can't get sysinfo's length");
		throw std::runtime_error("Can't get sysinfo's length");
	}

	std::vector<BYTE> buffer(systeminfo_len);
	status = Memory::ntdll_.NtQuerySystemInformation(
		SystemProcessInformation,
		buffer.data(),
		systeminfo_len,
		&systeminfo_len
	);

	if (!NT_SUCCESS(status)) {
		LOG_ERROR("Can't get system information");
		throw std::runtime_error("Can't get system information");
	}

	PSYSTEM_PROCESS_INFORMATION sysinfo =
		reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buffer.data());

	while (true) {
		processes.emplace_back(
			HandleToLong(sysinfo->UniqueProcessId), 
			UnicodeStringToWString(sysinfo->ImageName));

		if (sysinfo->NextEntryOffset == 0) {
			break;
		}
		sysinfo = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(
			reinterpret_cast<BYTE*>(sysinfo) + sysinfo->NextEntryOffset
			);
	}

	return processes;
}

std::optional<ProcessInfo> ProcessProvider::GetProcessByName(const std::wstring& name)
{
	return FindProcessByName(name);
}