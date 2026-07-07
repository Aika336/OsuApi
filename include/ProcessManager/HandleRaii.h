#pragma once

#include <Windows.h>

struct HandleRaii {
    HANDLE hProcess = NULL;

    HandleRaii() = default;
    HandleRaii(HANDLE h);

    HandleRaii(const HandleRaii&) = delete;
    HandleRaii& operator=(const HandleRaii&) = delete;

    HandleRaii(HandleRaii&& other) noexcept;

    HandleRaii& operator=(HandleRaii&& other) noexcept;

    ~HandleRaii();

private:
    void Close();
};