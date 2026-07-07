#include "HandlerRaii.h"

HandleRaii::HandleRaii(HANDLE h) : hProcess(h)
{
}

HandleRaii::HandleRaii(HandleRaii&& other) noexcept
    : hProcess(other.hProcess)
{
    other.hProcess = NULL;
}

HandleRaii& HandleRaii::operator=(HandleRaii&& other) noexcept
{
    if (this != &other) {
        Close();
        hProcess = other.hProcess;
        other.hProcess = NULL;
    }
    return *this;
}

HandleRaii::~HandleRaii()
{
    Close();
}

void HandleRaii::Close() {
    if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE) {
        CloseHandle(hProcess);
        hProcess = NULL;
    }
}