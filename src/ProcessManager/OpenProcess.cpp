#include "OpenProcess.h"

#include <iostream>

Handler_raii OpenProcess::OpenProcessByIdW(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}

Handler_raii OpenProcess::OpenProcessByIdR(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdR: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}

Handler_raii OpenProcess::OpenProcessByIdRW(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdRW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}
