#include "Stream/ByteStream.hpp"
#include <cstdint>
#include <cstring>
#include <string>

class ByteStream {
public:
    static constexpr size_t bufferSize = 1024;

    ByteStream() : position(0) {
        std::memset(buffer, 0, sizeof(buffer));
    }

    void reset() {
        position = 0;
        std::memset(buffer, 0, sizeof(buffer));
    }

    void seek(size_t pos) {
        position = pos;
    }

    void writeInt(int32_t value) {
        if (canWrite(4)) {
            buffer[position] = (value >> 24) & 0xFF;
            buffer[position + 1] = (value >> 16) & 0xFF;
            buffer[position + 2] = (value >> 8) & 0xFF;
            buffer[position + 3] = value & 0xFF;
            position += 4;
        }
    }

    int32_t readInt() {
        int32_t value = 0;
        if (canRead(4)) {
            value = (buffer[position] << 24) |
                    (buffer[position + 1] << 16) |
                    (buffer[position + 2] << 8) |
                    buffer[position + 3];
            position += 4;
        }
        return value;
    }

    void writeString(const std::string& value) {
        int len = value.size();
        writeInt(len);
        if (canWrite(len)) {
            std::copy(value.begin(), value.begin() + len, buffer + position);
            position += len;
        }
    }

    std::string readString() {
        uint32_t length = readInt();
        std::string result;
        if (canRead(length)) {
            result.assign(buffer + position, length);
            position += length;
        }
        return result;
    }

    void writeByte(int value) {
        if (canWrite(1)) {
            buffer[position++] = static_cast<uint8_t>(value);
        }
    }

    int readByte() {
        if (canRead(1)) {
            return buffer[position++];
        }
        return -1; // Or throw an exception
    }

    void writeVInt(int32_t value) {
        int32_t zigzagValue = (value << 1) ^ (value >> 31);
        while (zigzagValue >= 0x80) {
            writeByte((zigzagValue & 0x7F) | 0x80);
            zigzagValue >>= 7;
        }
        writeByte(zigzagValue & 0x7F);
    }

    int32_t readVInt() {
        int32_t result = 0;
        int shift = 0;
        int byte;
        do {
            byte = readByte();
            result |= (byte & 0x7F) << shift;
            shift += 7;
        } while (byte & 0x80);
        return (result >> 1) ^ (-(result & 1));
    }

private:
    uint8_t buffer[bufferSize];
    size_t position;

    [[nodiscard]] bool canRead(size_t size) const {
        return position + size <= bufferSize;
    }

    [[nodiscard]] bool canWrite(size_t size) const {
        return position + size <= bufferSize;
    }
};
