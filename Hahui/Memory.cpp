//
// Created by Aika on 28.06.2026.
//

#pragma once
#include <stdexcept>

#include "Memory.h"

inline HANDLE Memory::OpenProcess(DWORD pId) {
    return ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pId);
}

inline void Memory::CloseProcess(HANDLE hProcess) {
    if (hProcess != nullptr && hProcess != INVALID_HANDLE_VALUE) {
        CloseHandle(hProcess);
    }
}

template <typename T>
inline T Memory::RPM(HANDLE hProcess, uintptr_t address) {
    T value{};
    SIZE_T read_bytes = 0;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), &read_bytes) || read_bytes != sizeof(T)) {
        throw std::runtime_error("Memory::RPM: ReadProcessMemory failed");
    }
    return value;
}

inline std::vector<uint8_t> Memory::ReadMemoryRegion(HANDLE hProcess, uintptr_t address, size_t size) {
    std::vector<uint8_t> buffer(size);
    SIZE_T read_bytes_ = 0;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), buffer.data(), size, &read_bytes_)) {
        buffer.clear();
    }

    return buffer;
}

inline std::wstring Memory::ReadDotNetString(HANDLE hProcess, uintptr_t string_address) {
    if (string_address == 0)
        return L"";

    int32_t length = Memory::RPM<int32_t>(hProcess, string_address + 0x8);
    if (length <= 0 || length > 8192)
        return L"";

    std::wstring value(length, L'\0');
    SIZE_T bytesRead = 0;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(string_address + 0xC), &value[0], length * sizeof(wchar_t), &bytesRead)) {
        return L"";
    }

    return value;
}
