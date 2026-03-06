cmake_minimum_required(VERSION 3.17)
project(quicksort LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.4.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
FetchContent_MakeAvailable(googlebenchmark)

add_subdirectory(tests)
add_subdirectory(lib)