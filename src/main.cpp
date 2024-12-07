#include <iostream>
#include <chrono>
#include "Stream/ByteStream.hpp"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Hello, Core++!\n";
    ByteStream stream;
    
    stream.writeInt(1488);
    stream.seek(0);
    std::cout << stream.readInt() << std::endl;

    stream.reset();

    stream.writeString("Core++ idk");
    stream.seek(0);
    std::cout << stream.readString() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "took: " << duration.count() << " ms" << std::endl;
}