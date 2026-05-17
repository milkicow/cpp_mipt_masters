add_executable(palindrome_tests palindrome.cpp)

target_link_libraries(palindrome_tests PRIVATE palindrome GTest::gtest_main)
