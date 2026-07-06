#include "DotNetString.h"
#include "Memory.h"

std::optional<std::wstring> DotNetString::Read(const Handler_raii& handler, uintptr_t address) {
    if (address == 0) {
        return std::nullopt;
    }

    auto length = Memory::RPM<int32_t>(handler, address + kLengthOffset);
    if (!length || *length <= 0 || static_cast<uint32_t>(*length) > kMaxLength) {
        return std::nullopt;
    }

    const size_t byte_size = static_cast<size_t>(*length) * sizeof(wchar_t);
    auto raw_bytes = Memory::ReadMemoryRegion(handler, address + kDataOffset, byte_size);
    if (!raw_bytes || raw_bytes->size() != byte_size) {
        return std::nullopt;
    }

    std::wstring result(*length, L'\0');
    std::memcpy(result.data(), raw_bytes->data(), byte_size);
    return result;
}