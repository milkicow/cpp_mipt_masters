cmake_minimum_required(VERSION 3.17)
project(dll_example LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS filesystem)

add_library(shared_v1 SHARED lib_v1.cpp)
set_target_properties(shared_v1 PROPERTIES
    PREFIX      "lib"
    OUTPUT_NAME "shared_v1"
)

add_library(shared_v2 SHARED lib_v2.cpp)
set_target_properties(shared_v2 PROPERTIES
    PREFIX      "lib"
    OUTPUT_NAME "shared_v2"
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE
    Boost::filesystem
    ${CMAKE_DL_LIBS}
)

add_dependencies(main shared_v1 shared_v2)