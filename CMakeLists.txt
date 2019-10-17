cmake_minimum_required(VERSION 3.8)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -ltbb")
set(CMAKE_CXX_FLAGS_RELEASE  "-O2 -ftree-vectorize -march=native")

add_executable(parallel parallel.cpp)
add_executable(matmul matmul.cpp)
