#pragma once

#include <Windows.h>
#include <string>

struct ProcessInfo {
	DWORD processId = 0;
	std::wstring name = L"";
};