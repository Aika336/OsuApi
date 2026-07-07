#include "OpenProcess.h"

#include <iostream>

HandleRaii OpenProcess::OpenProcessForWrite(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	HandleRaii handler;
	handler.hProcess = hProcess;

	return handler;
}

HandleRaii OpenProcess::OpenProcessForRead(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdR: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	HandleRaii handler;
	handler.hProcess = hProcess;

	return handler;
}

HandleRaii OpenProcess::OpenProcessForReadWrite(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdRW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	HandleRaii handler;
	handler.hProcess = hProcess;

	return handler;
}
