add_executable(list_test list.cpp)

target_link_libraries(list_test PRIVATE list GTest::gtest_main)
