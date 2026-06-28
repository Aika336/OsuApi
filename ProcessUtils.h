//
// Created by Aika on 28.06.2026.
//

#pragma once
#include <vector>
#include <Windows.h>
#include <string>

class ProcessUtils {
public:
    static DWORD FindProcessId(const std::wstring& process_name);
    static std::vector<std::pair<std::wstring, DWORD>> EnumerateProcesses();
    static HANDLE OpenProcessByName(const std::wstring& process_name);
};