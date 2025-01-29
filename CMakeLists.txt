cmake_minimum_required(VERSION 3.10)

project(archiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(archiver
    main.cpp
    src/suffix_tree.cpp
    src/bwt.cpp
    src/mtf.cpp
    src/rle.cpp
    src/huffman.cpp)

include_directories(include)


