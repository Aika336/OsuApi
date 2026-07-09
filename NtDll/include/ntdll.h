#pragma once

#include "domain.h"

#include <Windows.h>
#include <winternl.h>

class NtDll {
    HMODULE ntdll_{ 0 };
    pNtQuerySystemInformation NtQuerySystemInformation{0};
    pNtOpenProcess NtOpenProcess{0};
    pNtReadProcessMemory NtReadProcessMemory{0};

	void LoadNtQuerySystemInformation();
	void LoadNtOpenProcess();
	void LoadNtReadProcessMemory();
public:
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
};