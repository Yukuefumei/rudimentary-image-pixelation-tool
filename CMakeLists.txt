cmake_minimum_required(VERSION 3.15)
project(PixelArtConverter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

include_directories(libs)

file(GLOB SOURCES "src/*.cpp")

add_executable(pixel_art_converter ${SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(pixel_art_converter m)
endif()