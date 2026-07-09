#include "ntdll.h"
#include "domain.h"

#include <Windows.h>

NtDll::NtDll() {
	ntdll_ = LoadModule(nt_functions_name::NTDLL.data());
}

void NtDll::LoadNtQuerySystemInformation()
{
	if (NtQuerySystemInformation_) {
		return;
	}
	NtQuerySystemInformation_ = LoadFunctionFromModule<pNtQuerySystemInformation>(ntdll_, nt_functions_name::NTQSI);
}

void NtDll::LoadNtOpenProcess()
{
	if (NtOpenProcess_) {
		return;
	}
	NtOpenProcess_ = LoadFunctionFromModule<pNtOpenProcess>(ntdll_, nt_functions_name::OPEN_PROCESS);
}

void NtDll::LoadNtReadProcessMemory()
{
	if (NtReadProcessMemory_) {
		return;
	}
	NtReadProcessMemory_ = LoadFunctionFromModule<pNtReadProcessMemory>(ntdll_, nt_functions_name::READ_MEMORY);
}

void NtDll::LoadNtQueryVirtualMemory()
{
	if (NtQueryVirtualMemory_) {
		return;
	}
	NtQueryVirtualMemory_ = LoadFunctionFromModule<pNtQueryVirtualMemory>(ntdll_, nt_functions_name::QUERY_MEMORY);
}

NTSTATUS NtDll::NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
	LoadNtQuerySystemInformation();
	return NtQuerySystemInformation_(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
}

NTSTATUS NtDll::NtOpenProcess(PHANDLE hProcess, ACCESS_MASK desired_access, POBJECT_ATTRIBUTES object_attributes, CLIENT_ID* client_id) {
	LoadNtOpenProcess();
	return NtOpenProcess_(hProcess, desired_access, object_attributes, client_id);
}

NTSTATUS NtDll::NtReadProcessMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, SIZE_T NumberOfBytesToRead, PSIZE_T NumberOfBytesRead)
{
	LoadNtReadProcessMemory();
	return NtReadProcessMemory_(ProcessHandle, BaseAddress, Buffer, NumberOfBytesToRead, NumberOfBytesRead);
}