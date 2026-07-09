#include "Memory.h"
#include "Logger.h"

#include <vector>

std::optional<std::vector<uint8_t>> Memory::ReadMemoryRegion(
	const HandleRaii& handle, 
	uintptr_t address, 
	size_t size) 
{
	std::vector<uint8_t> buffer(size);
	SIZE_T read_bytes = 0;

	NTSTATUS status = ntdll::GetNtDll().NtReadProcessMemory(
		handle.hProcess,
		reinterpret_cast<LPVOID>(address),
		buffer.data(),
		size,
		&read_bytes
	);

	if (!NT_SUCCESS(status)) {
		LOG_ERROR("Failed to read memory region at address: " + std::format("0x{:x}", address));
		return std::nullopt;
	}
	else if (buffer.size() != read_bytes) {
		LOG_ERROR("Read bytes != expected bytes: " + std::format("0x{:x}", address));
		return std::nullopt;
	}

	return buffer;
}