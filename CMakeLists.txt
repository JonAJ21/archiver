cmake_minimum_required(VERSION 3.10)

project(archiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(archiver
    main.cpp
    src/test.cpp)

include_directories(include)


