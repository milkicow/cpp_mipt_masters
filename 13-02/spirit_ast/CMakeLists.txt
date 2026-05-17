cmake_minimum_required(VERSION 3.17)
project(calculator_file_ast LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

configure_file(test.txt test.txt COPYONLY)
