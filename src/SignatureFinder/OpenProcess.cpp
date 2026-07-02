#include "OpenProcess.h"

#include <iostream>

HANDLE OpenProcess::OpenProcessByIdW(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return NULL;
	}

	return hProcess;
}

HANDLE OpenProcess::OpenProcessByIdR(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return NULL;
	}

	return hProcess;
}

HANDLE OpenProcess::OpenProcessByIdRW(DWORD processId)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << processId << ". Error: " << GetLastError() << std::endl;
		return NULL;
	}

	return hProcess;
}
