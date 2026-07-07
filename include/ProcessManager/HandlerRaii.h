#pragma once

#include <Windows.h>

struct HandleRaii {
    HANDLE hProcess = NULL;

    HandleRaii() = default;
    HandleRaii(HANDLE h) : hProcess(h) {}

    HandleRaii(const HandleRaii&) = delete;
    HandleRaii& operator=(const HandleRaii&) = delete;

    HandleRaii(HandleRaii&& other) noexcept : hProcess(other.hProcess) {
        other.hProcess = NULL;
    }

    HandleRaii& operator=(HandleRaii&& other) noexcept {
        if (this != &other) {
            Close();
            hProcess = other.hProcess;
            other.hProcess = NULL;
        }
        return *this;
    }

    ~HandleRaii() {
        Close();
    }

private:
    void Close() {
        if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE) {
            CloseHandle(hProcess);
            hProcess = NULL;
        }
    }
};