add_executable(pimpl_tests pimpl_tests.cpp)

target_link_libraries(pimpl_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)
