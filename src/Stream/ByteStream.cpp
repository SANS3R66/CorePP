#include "Stream/ByteStream.hpp"
#include <cstdint>
#include <cstring>

ByteStream::ByteStream()
{
    reset();
}

void ByteStream::reset()
{
    position = 0;
    memset(buffer, 0, sizeof(buffer));
}

void ByteStream::seek(long value)
{
    position = value;
}

void ByteStream::writeInt(int32_t value)
{
    buffer[position] = (value >> 24) & 0xFF;
    buffer[position + 1] = (value >> 16) & 0xFF;
    buffer[position + 2] = (value >> 8) & 0xFF;
    buffer[position + 3] = value & 0xFF;
    position += 4;
}

int32_t ByteStream::readInt()
{
    uint32_t value = 0;
    value |= ((uint32_t)(buffer[position]) << 24);
    value |= ((uint32_t)(buffer[position + 1]) << 16);
    value |= ((uint32_t)(buffer[position + 2]) << 8);
    value |= (uint32_t)(buffer[position + 3]);
    position += 4;
    return value;
}

void ByteStream::writeString(std::string value)
{
    writeInt(value.size());
    memcpy(buffer + position, value.c_str(), value.size());
    position += value.size();
}

std::string ByteStream::readString()
{
    uint32_t length = readInt();
    std::string result((char *)(buffer + position), length);
    position += length;
    return result;
}

void ByteStream::writeByte(int value)
{
    buffer[position] = (value & 0xFF);
}

int ByteStream::readByte()
{
    return buffer[position];
}
