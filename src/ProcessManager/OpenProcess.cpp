#include "OpenProcess.h"
#include "Logger.h"

#include <iostream>
#include <string>

HandleRaii OpenProcess::OpenProcessForWrite(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::string msg = "OpenProcessForWrite: Failed to open process with ID " + info.processId;
		msg += ". Error: " + GetLastError();
		LOG_ERROR(msg);
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
		std::string msg = "OpenProcessForRead: Failed to open process with ID " + info.processId;
		msg += ". Error: " + GetLastError();
		LOG_ERROR(msg);
		return {}; return {};
	}
	HandleRaii handler;
	handler.hProcess = hProcess;

	return handler;
}

HandleRaii OpenProcess::OpenProcessForReadWrite(ProcessInfo info)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, info.processId);
	if (hProcess == NULL) {
		std::string msg = "OpenProcessForReadWrite: Failed to open process with ID " + info.processId;
		msg += ". Error: " + GetLastError();
		LOG_ERROR(msg);
		return {}; return {};
	}
	HandleRaii handler;
	handler.hProcess = hProcess;

	return handler;
}
