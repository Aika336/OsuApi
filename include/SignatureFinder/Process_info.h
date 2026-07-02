#pragma once

#include <Windows.h>
#include <string>

struct ProcessInfo {
	DWORD processId;
	std::wstring name;
};