#pragma once

#include "ProcessMemoryScanner.h"
#include <Windows.h>

uintptr_t ProcessMemoryScanner::ScanProcessMemoryForSignature(Handler_raii handler, PatternMatcher mather)
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
                int offset = mather.CheckForStableSignature(regionBytes);
                if (offset >= 0) {
                    return static_cast<uintptr_t>((SIZE_T)mbi.BaseAddress + offset + mather.getOffset());
                }
            }
        }

		addr_scan = region_end;
	}
}
