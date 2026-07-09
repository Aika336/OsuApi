#include "ProcessMemoryScanner.h"
#include "Memory.h"

std::optional<uintptr_t> ProcessMemoryScanner::ScanProcessMemoryForSignature(const HandleRaii& handler, PatternMatcher mather)
{
	SIZE_T addr_scan = 0;
	MEMORY_BASIC_INFORMATION mbi;
    SIZE_T return_length = 0;


	while (NT_SUCCESS(NtDLL.NtQueryVirtualMemory(
        handler.hProcess,
        reinterpret_cast<LPVOID>(addr_scan),
        MemoryBasicInformation,
        &mbi,
        sizeof(mbi),
        &return_length
    ))) {
		SIZE_T region_end = reinterpret_cast<SIZE_T>(mbi.BaseAddress) + mbi.RegionSize;

        if (mbi.State == MEM_COMMIT &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)) {

            std::vector<uint8_t> regionBytes(mbi.RegionSize);
            SIZE_T bytes_read = 0;

            NTSTATUS status = NtDLL.NtReadProcessMemory(handler.hProcess, mbi.BaseAddress, regionBytes.data(), mbi.RegionSize, &bytes_read);

            if (NT_SUCCESS(status) && bytes_read > 0) {
                if (auto offset = mather.CheckForSignature(regionBytes)) {
                    return static_cast<uintptr_t>(
                        (SIZE_T)mbi.BaseAddress + 
                        offset.value() + 
                        mather.getOffset());
                }
            }
        }

		addr_scan = region_end;
	}

    return std::nullopt;
}
