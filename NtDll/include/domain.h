#pragma once

#include "Logger.h"

#include <Windows.h>
#include <winternl.h>

#include <string_view>
#include <minwindef.h>
#include <string>
#include <bcrypt.h>
#include <stdexcept>

namespace nt_functions_name {
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

std::wstring UnicodeStringToWString(const UNICODE_STRING& uStr);

inline HMODULE LoadModule(std::string_view module_name)
{
    if (module_name.empty()) {
        LOG("LoadModule: empty string module name");
        return 0;
    }

    HMODULE hModule = GetModuleHandleA(module_name.data());
    if (!hModule) {
        LOG_ERROR("Failed to load module: " + std::string(module_name));
        throw std::runtime_error("Failed to load module: " + std::string(module_name));
    }

    return hModule;
}

template<typename T>
T LoadFunctionFromModule(HMODULE hModuel, std::string_view function_name) {
    T func = reinterpret_cast<T>(GetProcAddress(hModuel, function_name.data()));
    if (!func) {
        LOG_ERROR("Incorrect load function: " + std::string(function_name));
        throw std::runtime_error("Incorrect load function: " + std::string(function_name));
    }

    return func;
}

typedef NTSTATUS(*pNtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

typedef NTSTATUS(*pNtReadProcessMemory) (
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    SIZE_T NumberOfBytesToRead,
    PSIZE_T NumberOfBytesRead
    );

typedef NTSTATUS(*pNtOpenProcess)(
    PHANDLE hProcess,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    CLIENT_ID* Client
    );