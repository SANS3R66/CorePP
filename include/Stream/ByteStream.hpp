#include <cstdint>
#include <cstring>
#include <iostream>

class ByteStream
{
public:
    static constexpr size_t bufferSize = 1024;

    ByteStream();

    void reset();
    void seek(size_t value);

    void writeInt(int32_t value);
    int32_t readInt();

    void writeString(const std::string &value);
    std::string readString();

    void writeByte(int value);
    int readByte();

    void writeVInt(int32_t a1);
    int32_t readVInt();

    [[nodiscard]] bool canRead(size_t size) const;
    [[nodiscard]] bool canWrite(size_t size) const;

private:
    uint8_t buffer[bufferSize];
    size_t position;
};