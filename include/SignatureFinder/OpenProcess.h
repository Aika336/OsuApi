#pragma once

#include <Windows.h>

class OpenProcess {
public:
	static HANDLE OpenProcessByIdW(DWORD processId);
	static HANDLE OpenProcessByIdR(DWORD processId);
	static HANDLE OpenProcessByIdRW(DWORD processId);
};