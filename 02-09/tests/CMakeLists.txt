add_executable(gcd_test gcd.cpp)
add_executable(lcm_test lcm.cpp)

target_link_libraries(gcd_test PRIVATE gcd_lcm GTest::gtest_main)
target_link_libraries(lcm_test PRIVATE gcd_lcm GTest::gtest_main)
