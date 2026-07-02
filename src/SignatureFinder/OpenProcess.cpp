#include "OpenProcess.h"

#include <iostream>

Handler_raii OpenProcess::OpenProcessByIdW(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}

Handler_raii OpenProcess::OpenProcessByIdR(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdR: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}

Handler_raii OpenProcess::OpenProcessByIdRW(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdRW: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	Handler_raii handler;
	handler.hProcess = hProcess;

	return handler;
}
