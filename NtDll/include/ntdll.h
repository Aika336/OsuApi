#pragma once

#include "domain.h"

#include <Windows.h>
#include <winternl.h>

class NtDll {
    HMODULE ntdll_{ 0 };
    pNtQuerySystemInformation NtQuerySystemInformation_{0};
    pNtOpenProcess NtOpenProcess_{0};
    pNtReadProcessMemory NtReadProcessMemory_{0};
    pNtQueryVirtualMemory NtQueryVirtualMemory_{0};

	void LoadNtQuerySystemInformation();
	void LoadNtOpenProcess();
	void LoadNtReadProcessMemory();
    void LoadNtQueryVirtualMemory();
public:
    NtDll();
    ~NtDll();
    NTSTATUS NtQuerySystemInformation(
        SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength
    );
    NTSTATUS NtOpenProcess(
        PHANDLE hProcess,
        ACCESS_MASK desired_access,
        POBJECT_ATTRIBUTES object_attributes,
        CLIENT_ID* client_id
    );
    NTSTATUS NtReadProcessMemory(
        HANDLE ProcessHandle,
        PVOID BaseAddress,
        PVOID Buffer,
        SIZE_T NumberOfBytesToRead,
        PSIZE_T NumberOfBytesRead
    );
    NTSTATUS NtQueryVirtualMemory(
        HANDLE ProcessHandle,
        PVOID BaseAddress,
        MEMORY_INFORMATION_CLASS MemoryInformationClass,
        PVOID MemoryInformation,
        SIZE_T MemoryInformationLength,
        PSIZE_T ReturnLength
    );
};