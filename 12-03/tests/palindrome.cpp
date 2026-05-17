#include "palindrome.hpp"

#include <gtest/gtest.h>

TEST(Palindrome, singleChar) { EXPECT_EQ("a", palindrome::longest("a")); }

TEST(Palindrome, empty) { EXPECT_EQ("", palindrome::longest("")); }

TEST(Palindrome, twoSameChars) { EXPECT_EQ("aa", palindrome::longest("aa")); }

TEST(Palindrome, twoDiffChars) { EXPECT_EQ(1u, palindrome::longest("ab").size()); }

TEST(Palindrome, oddLength) {
    auto result = palindrome::longest("babad");
    EXPECT_TRUE(result == "bab" || result == "aba");
}

TEST(Palindrome, evenLength) { EXPECT_EQ("bb", palindrome::longest("cbbd")); }

TEST(Palindrome, wholeString) { EXPECT_EQ("racecar", palindrome::longest("racecar")); }

TEST(Palindrome, longerString) { EXPECT_EQ("anana", palindrome::longest("bananas")); }

TEST(Palindrome, allSameChars) { EXPECT_EQ("aaaa", palindrome::longest("aaaa")); }

TEST(Palindrome, palindromeAtEnd) { EXPECT_EQ("aba", palindrome::longest("xyzaba")); }

TEST(Palindrome, palindromeAtStart) { EXPECT_EQ("aba", palindrome::longest("abaxyz")); }
