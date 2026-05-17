add_executable(fibonacci_tests fibonacci_tests.cpp)

target_link_libraries(fibonacci_tests PRIVATE fibonacci GTest::gtest_main)
