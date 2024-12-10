#include <iostream>
#include <chrono>
#include "Stream/ByteStream.hpp"

int main(int argc, char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    
    ByteStream stream;
    
    stream.writeInt(1488);
    stream.seek(0);
    std::cout << stream.readInt() << std::endl;

    stream.reset();

    stream.writeString("Core++ idk");
    stream.seek(0);
    std::cout << stream.readString() << std::endl;

    stream.reset();

    stream.writeByte(255);
    stream.seek(0);
    std::cout << stream.readByte() << std::endl;

    stream.reset();

    stream.writeVInt(658067456);
    stream.seek(0);
    std::cout << stream.readVInt() << std::endl;

    stream.reset();

    stream.writeBoolean(true);
    stream.seek(0);
    std::cout << std::boolalpha << stream.readBoolean() << std::endl;

    stream.reset();

    stream.writeLong(1234, 5678);
    stream.seek(0);

    int32_t int1;
    int32_t int2;

    stream.readLong(int1, int2);
    std::cout << int1 << " " <<int2 << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "took: " << duration.count() << " ms" << std::endl;
}