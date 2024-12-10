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
    void writeString();
    std::string readString();

    void writeByte(int value);
    int readByte();

    void writeVInt(int32_t value);
    int32_t readVInt();

    void writeBoolean(bool value);
    bool readBoolean();

    void writeLong(int32_t value1, int32_t value2);
    void readLong(int32_t& retval1, int32_t& retval2);

    void writeDataReference(int32_t value1, int32_t value2);
    void readDataReference(int32_t& retval1, int32_t retval2);

    [[nodiscard]] bool canRead(size_t size) const;
    [[nodiscard]] bool canWrite(size_t size) const;

private:
    uint8_t buffer[bufferSize];
    size_t position;
};