#include "ntdll.h"
#include "domain.h"

NtDll::NtDll() {
	ntdll_ = LoadModule(nt_functions_name::NTDLL.data());
}