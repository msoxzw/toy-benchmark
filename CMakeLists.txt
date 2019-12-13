cmake_minimum_required(VERSION 3.8)
project(benchmark)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-pipe -march=native)
link_libraries(tbb)

add_executable(parallel parallel.cpp)
add_executable(matmul matmul.cpp)
