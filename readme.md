# Core++,
a core server for brawl stars.

shitty ass english and C++ code is expected, cause im russian and coding first time in C++

# requirements
## Linux
idk just get g++ (should be already installed on Linux), cmake and ninja

## Windows
get MSYS2 MinGW64

install in MinGW64 terminal some packages:
```
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake
```

add msys2\usr\bin; msys2\mingw64\bin in the PATH

#### done!! good job!!

## building
firstly, you need to initialize the project; you dont need to initialize it everytime you building
```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

after that,

input in the terminal:
```
cmake --build build
```