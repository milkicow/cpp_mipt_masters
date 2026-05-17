add_executable(ranges_tests ranges_tests.cpp)

target_link_libraries(ranges_tests PRIVATE ranges_utils GTest::gtest_main)
