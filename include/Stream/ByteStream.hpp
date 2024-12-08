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

    void writeByte(int value);
    int readByte();
    
    void writeVInt(int32_t a1);
    int32_t readVInt();

    bool canRead(int a1);
    bool canWrite(int a1);
};