add_executable(rectangle_test rectangle.cpp)

target_link_libraries(rectangle_test PRIVATE rectangle GTest::gtest_main)
