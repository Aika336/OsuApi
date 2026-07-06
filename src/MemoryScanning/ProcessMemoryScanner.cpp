#include "ProcessMemoryScanner.h"
#include <Windows.h>

std::optional<uintptr_t> ProcessMemoryScanner::ScanProcessMemoryForSignature(const Handler_raii& handler, PatternMatcher mather)
{
	SIZE_T addr_scan = 0;
	MEMORY_BASIC_INFORMATION mbi;
	while (VirtualQueryEx(handler.hProcess, reinterpret_cast<LPVOID>(addr_scan), &mbi, sizeof(mbi))) {
		SIZE_T region_end = reinterpret_cast<SIZE_T>(mbi.BaseAddress) + mbi.RegionSize;

        if (mbi.State == MEM_COMMIT &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)) {

            std::vector<uint8_t> regionBytes(mbi.RegionSize);
            SIZE_T bytes_read = 0;
            if (ReadProcessMemory(handler.hProcess, mbi.BaseAddress, regionBytes.data(), mbi.RegionSize, &bytes_read) > 0) {
                if (auto offset = mather.CheckForStableSignature(regionBytes)) {
                    return static_cast<uintptr_t>((SIZE_T)mbi.BaseAddress + offset.value() + mather.getOffset());
                }
            }
        }

		addr_scan = region_end;
	}

    return std::nullopt;
}
