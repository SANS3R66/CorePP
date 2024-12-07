file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(CorePP ${SOURCES})

target_include_directories(CorePP PRIVATE ${CMAKE_SOURCE_DIR}/include)