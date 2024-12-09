#include "Stream/ByteStream.hpp"
#include <cstdint>
#include <cstring>
#include <string>

ByteStream::ByteStream() : position(0)
{
    std::memset(buffer, 0, sizeof(buffer));
}

void ByteStream::reset()
{
    position = 0;
    std::memset(buffer, 0, sizeof(buffer));
}

void ByteStream::seek(size_t pos)
{
    position = pos;
}

void ByteStream::writeInt(int32_t value)
{
    if (canWrite(4))
    {
        buffer[position] = (value >> 24) & 0xFF;
        buffer[position + 1] = (value >> 16) & 0xFF;
        buffer[position + 2] = (value >> 8) & 0xFF;
        buffer[position + 3] = value & 0xFF;
        position += 4;
    }
}

int32_t ByteStream::readInt()
{
    int32_t value = 0;
    if (canRead(4))
    {
        value = (buffer[position] << 24) |
                (buffer[position + 1] << 16) |
                (buffer[position + 2] << 8) |
                buffer[position + 3];
        position += 4;
    }
    return value;
}

void ByteStream::writeString(const std::string &value)
{
    int len = value.size();
    writeInt(len);
    if (canWrite(len))
    {
        std::copy(value.begin(), value.begin() + len, buffer + position);
        position += len;
    }
}

std::string ByteStream::readString()
{
    uint32_t length = readInt();
    std::string result;
    if (canRead(length))
    {
        result.assign((char *)buffer + position, length);
        position += length;
    }
    return result;
}

void ByteStream::writeByte(int value)
{
    if (canWrite(1))
    {
        buffer[position++] = static_cast<uint8_t>(value);
    }
}

int ByteStream::readByte()
{
    if (canRead(1))
    {
        return buffer[position++];
    }
    return -1;
}

void ByteStream::writeVInt(int32_t value)
{
    int32_t zigzagValue = (value << 1) ^ (value >> 31);
    while (zigzagValue >= 0x80)
    {
        writeByte((zigzagValue & 0x7F) | 0x80);
        zigzagValue >>= 7;
    }
    writeByte(zigzagValue & 0x7F);
}

int32_t ByteStream::readVInt()
{
    int32_t result = 0;
    int shift = 0;
    int byte;
    do
    {
        byte = readByte();
        result |= (byte & 0x7F) << shift;
        shift += 7;
    } while (byte & 0x80);
    return (result >> 1) ^ (-(result & 1));
}

[[nodiscard]] bool ByteStream::canRead(size_t size) const
{
    return position + size <= bufferSize;
}

[[nodiscard]] bool ByteStream::canWrite(size_t size) const
{
    return position + size <= bufferSize;
}