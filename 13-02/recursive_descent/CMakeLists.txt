cmake_minimum_required(VERSION 3.17)
project(calculator_file LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

configure_file(test.txt test.txt COPYONLY)
