#pragma once

#include "Logger.h"

#include <Windows.h>
#include <winternl.h>

#include <string_view>
#include <minwindef.h>
#include <string>
#include <bcrypt.h>

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

HMODULE LoadModule(std::string_view module_name);
std::wstring UnicodeStringToWString(const UNICODE_STRING& uStr);

template<typename T>
T LoadFunctionFromModule(HMODULE hModuel, std::string_view function_name) {
    T func = reinterpret_cast<T>(GetProcAddress(hModuel, function_name));
    if (!func) {
        LOG("Incorrect load function: " + std::string(function_name));
        throw std::runtime_error("Incorrect load function: " + std::string(function_name));
    }

    return func;
}