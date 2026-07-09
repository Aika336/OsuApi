#include "domain.h"

inline std::wstring UnicodeStringToWString(const UNICODE_STRING& uStr)
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