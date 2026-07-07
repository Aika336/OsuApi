#include "DotNetString.h"
#include "Memory.h"
#include "Logger.h"

std::optional<std::wstring> DotNetString::Read(const HandleRaii& handler, uintptr_t address) {
    // если переданный адресс равгяеться 0, значит поле которое требуеться прочитать еще пустое
    if (address == 0) {
        return std::nullopt;
    }

    auto length = Memory::ReadAs<int32_t>(handler, address + k_kength_offset_);
    // !length не смог прочитать значения | *length <= 0, адресс прочитан но там мусор | > kMaxLength похоже на строку, но неправдоподобнной длинны
    if (!length || *length <= 0 || static_cast<uint32_t>(*length) > k_max_length_) {
        LOG_ERROR("DotNetString::Read: failed to read at " + address + k_kength_offset_);
        return std::nullopt;
    }

    const size_t byte_size = static_cast<size_t>(*length) * sizeof(wchar_t);
    auto raw_bytes = Memory::ReadMemoryRegion(handler, address + k_data_offset_, byte_size);
    if (!raw_bytes || raw_bytes->size() != byte_size) {
        std::string msg = "DotNetString::Read: failed to read " + byte_size;
        msg += " bytes of string data at " + address + k_data_offset_;
        LOG_ERROR(msg);
        return std::nullopt;
    }

    std::wstring result(*length, L'\0');
    std::memcpy(result.data(), raw_bytes->data(), byte_size);
    return result;
}