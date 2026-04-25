#include <gtest/gtest.h>

#include "impl.hpp"
#include "pimpl.hpp"

TEST(PimplTest, Construction) {
    Entity a;
    ASSERT_EQ(a.get()->test(), 0x67);
}

TEST(PimplTest, MoveConstruction) {
    Entity a;
    ASSERT_EQ(a.get()->test(), 0x67);

    Entity b(std::move(a));
    ASSERT_EQ(b.get()->test(), 0x67);
}

TEST(PimplTest, MoveAssignment) {
    Entity a;
    ASSERT_EQ(a.get()->test(), 0x67);

    Entity c;
    c = std::move(a);
    ASSERT_EQ(c.get()->test(), 0x67);
}