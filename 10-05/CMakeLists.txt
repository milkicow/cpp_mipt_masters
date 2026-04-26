cmake_minimum_required(VERSION 3.17)
project(hash_cmp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -m32)
target_link_options(${PROJECT_NAME} PRIVATE -m32)