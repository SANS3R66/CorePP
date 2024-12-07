#include <cstdint>
#include <cstring>
#include <iostream>

class ByteStream
{
private:
    unsigned char buffer[1024];
    long position;
public:
    ByteStream();

    void reset();
    void seek(long value);

    void writeInt(int32_t value);
    int32_t readInt();

    void writeString(std::string value);
    std::string readString();
};