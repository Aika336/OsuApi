#pragma once

#include <string_view>

namespace name_dll {
    static constexpr std::string_view NTDLL = "ntdll.dll";
    static constexpr std::string_view ADJUST_PRIVILEGE = "NtAdjustPrivilegesToken";
    static constexpr std::string_view NTQSI = "NtQuerySystemInformation";
    static constexpr std::string_view OPEN_PROCESS = "NtOpenProcess";
    static constexpr std::string_view OPEN_PROCESS_TOKEN = "NtOpenProcessToken";
    static constexpr std::string_view NTQIT = "NtQueryInformationToken";
    static constexpr std::string_view BSOD = "NtRaiseHardError";
    static constexpr std::string_view READ_MEMORY = "NtReadVirtualMemory";
    static constexpr std::string_view QUERY_MEMORY = "NtQueryVirtualMemory";
}