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

void ByteStream::writeInt(int32_t value = 0)
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

void ByteStream::writeString() // null string
{
    writeInt(-1);
}

std::string ByteStream::readString()
{
    uint32_t length = readInt();
    std::string result;
    if (canRead(length) && length > 0)
    {
        result.assign((char *)buffer + position, length);
        position += length;
    }
    return result;
}

void ByteStream::writeByte(int value = 0)
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

void ByteStream::writeVInt(int32_t value = 0) // max number is 658067456 btw (or im poopoo coder sorr)
{
    int32_t temp = (value >> 25) & 0x40;
    int32_t flipped = value ^ (value >> 31);
    temp |= value & 0x3F;
    value >>= 6;
    flipped >>= 6;

    if (flipped == 0)
    {
        writeByte(temp);
        return;
    }

    writeByte(temp | 0x80);

    flipped >>= 7;
    int32_t r = 0;
    if (flipped != 0)
    {
        r = 0x80;
    }
    writeByte((value & 0x7F) | r);
    value >>= 7;

    while (flipped != 0)
    {
        flipped >>= 7;
        r = 0;
        if (flipped != 0)
        {
            r = 0x80;
        }
        writeByte((value & 0x7F) | r);
        value >>= 7;
    }
}

int32_t ByteStream::readVInt()
{
    int32_t result = 0;
    int shift = 0;
    int byte;
    do
    {
        byte = readByte();
        if (shift == 0)
        {
            int a1 = (byte & 0x40) >> 6;
            int a2 = (byte & 0x80) >> 7;
            int s = (byte << 1) & 0x7E;
            byte = s | (a2 << 7) | a1;
        }
        result |= (byte & 0x7F) << shift;
        shift += 7;
    } while (byte & 0x80);
    return (result >> 1) ^ (-(result & 1));
}

void ByteStream::writeBoolean(bool value = false)
{
    if (canWrite(1))
    {
        buffer[position++] = value ? 1 : 0;
    }
}

bool ByteStream::readBoolean()
{
    if (canRead(1))
    {
        return buffer[position++] != 0;
    }
    return false;
}

void ByteStream::writeLong(int32_t value1 = 0, int32_t value2 = 0) // basically 2 ints
{
    writeInt(value1);
    writeInt(value2);
}

void ByteStream::readLong(int32_t &retval1, int32_t &retval2)
{
    retval1 = readInt();
    retval2 = readInt();
}

void ByteStream::writeDataReference(int32_t value1 = 0, int32_t value2 = 0)
{
    if (value1 == 0) {
        writeVInt(0);
    } else {
        writeVInt(value1);
        writeVInt(value2);
    }
}

[[nodiscard]] bool ByteStream::canRead(size_t size) const
{
    return position + size <= bufferSize;
}

[[nodiscard]] bool ByteStream::canWrite(size_t size) const
{
    return position + size <= bufferSize;
}