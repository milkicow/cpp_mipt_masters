cmake_minimum_required(VERSION 3.17)
project(stack LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(stack_pair stack_pair.cpp)
add_executable(stack stack.cpp)
