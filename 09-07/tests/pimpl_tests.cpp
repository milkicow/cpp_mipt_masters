#include <gtest/gtest.h>

#include "pimpl.hpp"

TEST(PimplTest, Construction) {
    Entity a;
    ASSERT_EQ(a.test(), 0x67);
}

TEST(PimplTest, MoveConstruction) {
    Entity a;
    ASSERT_EQ(a.test(), 0x67);

    Entity b(std::move(a));
    ASSERT_EQ(b.test(), 0x67);
}

TEST(PimplTest, MoveAssignment) {
    Entity a;
    ASSERT_EQ(a.test(), 0x67);

    Entity c;
    c = std::move(a);
    ASSERT_EQ(c.test(), 0x67);
}