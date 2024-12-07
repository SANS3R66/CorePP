#include <cstdint>
#include <cstring>

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
};