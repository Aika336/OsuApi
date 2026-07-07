#pragma once

#include "../ProcessManager/HandleRaii.h"

#include <optional>
#include <string>
#include <cstdint>

class DotNetString {
public:
    static std::optional<std::wstring> Read(const HandleRaii& handler, uintptr_t address);

private:
    static constexpr uint32_t k_kength_offset_ = 0x8;
    static constexpr uint32_t k_data_offset_ = 0xC;
    static constexpr uint32_t k_max_length_ = 4096;
};