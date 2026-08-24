#include "domain.h"

std::wstring UnicodeStringToWString(const UNICODE_STRING& uStr)
{
    if (uStr.Buffer == nullptr || uStr.Length == 0) {
        return L"";
    }

    size_t cch = uStr.Length / sizeof(wchar_t);

    if (uStr.Length % sizeof(wchar_t) != 0) {
        cch = uStr.Length / sizeof(wchar_t);
    }

    return std::wstring(uStr.Buffer, cch);
}

HMODULE LoadModule(std::string_view module_name)
{
    if (module_name.empty()) {
        LOG("Empty string module name");
        return 0;
    }

    HMODULE hModule = GetModuleHandleA(module_name.data());
    if (!hModule) {
        LOG_ERROR("Failed to load module: " + std::string(module_name));
        throw std::runtime_error("Failed to load module: " + std::string(module_name));
    }

    return hModule;
}
