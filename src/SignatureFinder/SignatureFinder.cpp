#include "SignatureFinder.h"

int SignatureFinder::CheckForStableSignature(const std::vector<uint8_t>& bytes_region)
{
    const size_t region_size = bytes_region.size();
    const size_t sig_size = signature.size();

    if (sig_size == 0 || region_size < sig_size) return -1;

    const uint8_t* data = bytes_region.data();
    const uint8_t* sig = signature.data();

    // Предварительный подсчет константных байтов в сигнатуре
    size_t const_count = 0;
    for (size_t i = 0; i < sig_size; ++i) {
        if (sig[i] != NONCONST_BYTE) const_count++;
    }

    // Если все байты wildcard
    if (const_count == 0) {
        return static_cast<int>(value_offset);
    }

    // Минимальное количество совпадений, необходимое для успеха
    const size_t min_matches = const_count;

    const size_t max_pos = region_size - sig_size;

    for (size_t pos = 0; pos <= max_pos; ++pos) {
        // Быстрое сравнение с подсчетом несовпадений
        size_t mismatches = 0;
        bool found = true;

        for (size_t i = 0; i < sig_size; ++i) {
            if (sig[i] != NONCONST_BYTE && data[pos + i] != sig[i]) {
                mismatches++;
                if (mismatches > 0) {
                    found = false;
                    break;
                }
            }
        }

        if (found) {
            // std::cout << "[DBG] Signature found at chunk pos " << pos << std::endl;
            return static_cast<int>(pos + value_offset);
        }
    }

    return -1;
}

uintptr_t SignatureFinder::FindAddressBySignature()
{
    SIZE_T scan_addr = 0;
    MEMORY_BASIC_INFORMATION mbi;

    while (VirtualQueryEx(handler.hProcess, reinterpret_cast<LPVOID>(scan_addr), &mbi, sizeof(mbi))) {
		SIZE_T region_end = reinterpret_cast<SIZE_T>(mbi.BaseAddress) + mbi.RegionSize;

        if (mbi.State == MEM_COMMIT &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)) {

            std::vector<uint8_t> regionBytes(mbi.RegionSize);
            SIZE_T bytes_read = 0;
            if (ReadProcessMemory(handler.hProcess, mbi.BaseAddress, regionBytes.data(), mbi.RegionSize, &bytes_read) > 0) {
                int offset = CheckForStableSignature(regionBytes);
                if (offset >= 0) {
                    return static_cast<uintptr_t>((SIZE_T)mbi.BaseAddress + offset);
                }
            }
        }

        scan_addr = region_end;
    }
    return 0;
}