add_executable(quicksort_test quicksort.cpp)

target_link_libraries(quicksort_test PRIVATE quicksort GTest::gtest_main)
