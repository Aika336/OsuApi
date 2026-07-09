#include "ProcessOpener.h"
#include "Logger.h"
#include "ntdll.h"

#include <string>
#include <format>

HandleRaii ProcessOpener::OpenProcessForWrite(ProcessInfo info)
{
	CLIENT_ID client_id{ reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(info.processId)), nullptr };
	OBJECT_ATTRIBUTES attributes = { sizeof(attributes) };
	HandleRaii handle{0};

	NTSTATUS status = NtDLL.NtOpenProcess(
		&(handle.hProcess),
		PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION,
		&attributes,
		&client_id
	);

	if (!NT_SUCCESS(status)) {
		std::string msg = "NtOpenProcess failed for PID " + std::to_string(info.processId);
		msg += " with status: " + std::format("0x{:x}", status);
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}
	else if (!handle.hProcess || handle.hProcess == INVALID_HANDLE_VALUE) {
		std::string msg = "Error open process #" + std::string(info.name.begin(), info.name.end());
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}

	return handle;
}

HandleRaii ProcessOpener::OpenProcessForRead(ProcessInfo info)
{
	CLIENT_ID client_id{ reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(info.processId)), nullptr };
	OBJECT_ATTRIBUTES attributes = { sizeof(attributes) };
	HandleRaii handle{ 0 };

	NTSTATUS status = NtDLL.NtOpenProcess(
		&(handle.hProcess),
		PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		&attributes,
		&client_id
	);

	if (!NT_SUCCESS(status)) {
		std::string msg = "NtOpenProcess failed for PID " + std::to_string(info.processId);
		msg += " with status: " + std::format("0x{:x}", status);
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}
	else if (!handle.hProcess || handle.hProcess == INVALID_HANDLE_VALUE) {
		std::string msg = "Error open process #" + std::string(info.name.begin(), info.name.end());
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}

	return handle;
}

HandleRaii ProcessOpener::OpenProcessForReadWrite(ProcessInfo info)
{
	CLIENT_ID client_id{ reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(info.processId)), nullptr };
	OBJECT_ATTRIBUTES attributes = { sizeof(attributes) };
	HandleRaii handle{ 0 };

	NTSTATUS status = NtDLL.NtOpenProcess(
		&(handle.hProcess),
		PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		&attributes,
		&client_id
	);

	if (!NT_SUCCESS(status)) {
		std::string msg = "NtOpenProcess failed for PID " + std::to_string(info.processId);
		msg += " with status: " + std::format("0x{:x}", status);
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}
	else if (!handle.hProcess || handle.hProcess == INVALID_HANDLE_VALUE) {
		std::string msg = "Error open process #" + std::string(info.name.begin(), info.name.end());
		LOG_ERROR(msg);
		throw std::runtime_error(msg);
	}

	return handle;
}
