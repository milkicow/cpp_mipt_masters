#include "list.hpp"

#include <gtest/gtest.h>

TEST(list_test, empty_list) {
    List<int> list;
    ASSERT_EQ(std::begin(list), std::end(list));
}

TEST(list_test, postfix_increment) {
    List<int> list;
    list.push_back(5);
    list.push_back(6);

    auto it = std::begin(list);
    auto copy = it++;
    ASSERT_EQ(*copy, 5);
    ASSERT_EQ(*it, 6);
}

TEST(list_test, prefix_decrement) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    auto it = std::begin(list);
    ++it;
    ++it;

    ASSERT_EQ(*it, 3);
    --it;
    ASSERT_EQ(*it, 2);
    --it;
    ASSERT_EQ(*it, 1);
}

TEST(list_test, postfix_decrement) {
    List<int> list;
    list.push_back(10);
    list.push_back(20);

    auto it = std::begin(list);
    ++it;

    auto copy = it--;
    ASSERT_EQ(*copy, 20);
    ASSERT_EQ(*it, 10);
}
