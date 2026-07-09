#include "ntdll.h"
#include "domain.h"

NtDll::NtDll() {
	ntdll_ = LoadModule(nt_functions_name::NTDLL.data());
}

void NtDll::LoadNtQuerySystemInformation()
{
	if (!NtQuerySystemInformation_) {
		return;
	}
	NtQuerySystemInformation_ = LoadFunctionFromModule<pNtQuerySystemInformation>(ntdll_, nt_functions_name::NTQSI);
}

void NtDll::LoadNtOpenProcess()
{
	if (!NtOpenProcess_) {
		return;
	}
	NtOpenProcess_ = LoadFunctionFromModule<pNtOpenProcess>(ntdll_, nt_functions_name::OPEN_PROCESS);
}

void NtDll::LoadNtReadProcessMemory()
{
	if (!NtReadProcessMemory_) {
		return;
	}
	NtReadProcessMemory_ = LoadFunctionFromModule<pNtReadProcessMemory>(ntdll_, nt_functions_name::READ_MEMORY);
}