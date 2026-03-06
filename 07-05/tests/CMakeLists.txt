add_executable(test_quicksort quicksort.cpp)
target_link_libraries(test_quicksort PRIVATE quicksort GTest::gtest_main)
add_test(NAME test_quicksort COMMAND test_quicksort)

add_executable(bench_quicksort benchmark.cpp)
target_link_libraries(bench_quicksort PRIVATE quicksort benchmark::benchmark_main)