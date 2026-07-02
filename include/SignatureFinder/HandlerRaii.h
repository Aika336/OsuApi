#pragma once

#include <Windows.h>

struct Handler_raii {
    HANDLE hProcess = NULL;

    Handler_raii() = default;
    Handler_raii(const Handler_raii&) = delete;
    Handler_raii& operator=(const Handler_raii&) = delete;

	Handler_raii(Handler_raii&& other) noexcept : hProcess(other.hProcess) {
		other.hProcess = NULL;
	}

	Handler_raii& operator=(Handler_raii&& other) noexcept {
		if (this != &other) {
			if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE) {
				CloseHandle(hProcess);
				hProcess = NULL;
			}
			hProcess = other.hProcess;
			other.hProcess = NULL;
		}
		return *this;
	}

    ~Handler_raii() {
        if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE) {
            CloseHandle(hProcess);
            hProcess = NULL;
        }
    }
};