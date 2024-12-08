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
    uint32_t value = 0;

    if (canRead(4))
    {
        value |= ((uint32_t)(buffer[position]) << 24);
        value |= ((uint32_t)(buffer[position + 1]) << 16);
        value |= ((uint32_t)(buffer[position + 2]) << 8);
        value |= (uint32_t)(buffer[position + 3]);
        position += 4;
    }
    return value;
}

void ByteStream::writeString(std::string value)
{
    int len = value.size();
    writeInt(len);
    if (canWrite(len))
    {
        memcpy(buffer + position, value.c_str(), len);
        position += len;
    }
}

std::string ByteStream::readString()
{
    uint32_t length = readInt();
    std::string result = "";
    if (canRead(length))
    {
        result = std::string((char *)(buffer + position), length);
        position += length;
    }
    return result;
}

void ByteStream::writeByte(int value)
{
    if (canWrite(1))
    {
        buffer[position] = (value & 0xFF);
        position += 1;
    }
}

int ByteStream::readByte()
{
    int result;
    if (canRead(1))
    {
        result = buffer[position];
        position += 1;
    }
    return result;
}

void ByteStream::writeVInt(int32_t value)
{
    int32_t zigzagValue = (value << 1) ^ (value >> 31);

    while (zigzagValue > 0x7F)
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

    while (true)
    {
        byte = readByte();
        result |= (byte & 0x7F) << shift;
        shift += 7;

        if (!(byte & 0x80))
        {
            break;
        }
    }

    return (result >> 1) ^ (-(result & 1));
}

bool ByteStream::canRead(int a1)
{
    if (sizeof(buffer) - (position + a1) >= 0)
    {
        return true;
    }
    return false;
}

bool ByteStream::canWrite(int a1)
{
    if ((position + a1) - 1024 > 0) // 1024 - max offset
    {
        return false;
    }
    return true;
}