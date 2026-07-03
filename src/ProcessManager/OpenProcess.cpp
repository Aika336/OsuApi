#include "OpenProcess.h"

#include <iostream>

std::shared_ptr<Handler_raii> OpenProcess::OpenProcessByIdW(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	std::shared_ptr<Handler_raii> handler = std::make_shared<Handler_raii>();
	handler->hProcess = hProcess;

	return handler;
}

std::shared_ptr<Handler_raii> OpenProcess::OpenProcessByIdR(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdR: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	std::shared_ptr<Handler_raii> handler = std::make_shared<Handler_raii>();
	handler->hProcess = hProcess;

	return handler;
}

std::shared_ptr<Handler_raii> OpenProcess::OpenProcessByIdRW(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::cout << "OpenProcessByIdRW: Failed to open process with ID " << info.processId << ". Error: " << GetLastError() << std::endl;
		return {};
	}
	std::shared_ptr<Handler_raii> handler = std::make_shared<Handler_raii>();
	handler->hProcess = hProcess;

	return handler;
}
