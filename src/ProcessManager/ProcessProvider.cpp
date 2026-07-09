#include "ProcessProvider.h"
#include "Logger.h"
#include "ntdll.h"

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

	LOG_ERROR("ProvideProcesses::FindProcessByName: No find process by name " + std::string(name.begin(), name.end()));
	throw std::runtime_error("ProvideProcesses::FindProcessByName: No find process by name " + std::string(name.begin(), name.end()));
}

std::vector<ProcessInfo> ProcessProvider::GetAllProcesses() {
	std::vector<ProcessInfo> processes;
	
	ULONG systeminfo_len = 0;
	NTSTATUS status = ntdll::GetNtDll().NtQuerySystemInformation(
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
	status = ntdll::GetNtDll().NtQuerySystemInformation(
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