//
// Created by Aika on 28.06.2026.
//

#pragma once
#include "ProcessUtils.h"
#include <tlhelp32.h>

inline std::vector<std::pair<std::wstring, DWORD>> ProcessUtils::EnumerateProcesses() {
    std::vector<std::pair<std::wstring, DWORD>> processes;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return processes;

    PROCESSENTRY32W pe = { sizeof(pe) };
    if (Process32FirstW(snapshot, &pe)) {
        do {
            processes.emplace_back(pe.szExeFile, pe.th32ProcessID);
        } while (Process32NextW(snapshot, &pe));
    }
    CloseHandle(snapshot);
    return processes;
}

inline HANDLE ProcessUtils::OpenProcessByName(const std::wstring& process_name) {
    DWORD pId = FindProcessId(process_name);
    if (pId == 0) return INVALID_HANDLE_VALUE;
    return ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pId);
}

inline DWORD ProcessUtils::FindProcessId(const std::wstring& process_name) {
    std::vector<std::pair<std::wstring, DWORD>> processes = ProcessUtils::EnumerateProcesses();
    for (const auto& process : processes) {
        if (process.first == process_name) return process.second;
    }
    return 0;
}
