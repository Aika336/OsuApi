#pragma once

#include <Windows.h>

struct Handler_raii {
    HANDLE hProcess;
    ~Handler_raii() {
        if (hProcess != NULL || hProcess == INVALID_HANDLE_VALUE) {
            CloseHandle(hProcess);
        }
    }
};