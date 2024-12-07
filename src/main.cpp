#include <iostream>
#include "Stream/ByteStream.hpp"

int main()
{
    std::cout << "Hello, Core++!\n";
    ByteStream stream;
    
    stream.writeInt(1488);
    stream.seek(0);
    std::cout << stream.readInt() << std::endl;
}