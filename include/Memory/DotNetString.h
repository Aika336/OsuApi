#pragma once
#include "../ProcessManager/HandlerRaii.h"
#include <optional>
#include <string>
#include <cstdint>

class DotNetString {
public:
    static std::optional<std::wstring> Read(const Handler_raii& handler, uintptr_t address);

private:
    static constexpr uint32_t kLengthOffset = 0x8;
    static constexpr uint32_t kDataOffset = 0xC;
    static constexpr uint32_t kMaxLength = 4096;
};