#include "rectangle.hpp"

#include <gtest/gtest.h>

using namespace rectangle;

// Tests for intersection of multiple rectangles
TEST(RectangleIntersectionTest, EmptyVector) {
    std::vector<Rectangle> empty;
    Rectangle result = intersection(empty);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, SingleRectangle) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.square(), 100);
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
}

TEST(RectangleIntersectionTest, TwoRectanglesIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 5);
    EXPECT_EQ(result.left_top().y, 5);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
    EXPECT_EQ(result.square(), 25);
}

TEST(RectangleIntersectionTest, ThreeRectanglesIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15),
                                    Rectangle(8, 8, 12, 12)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 8);
    EXPECT_EQ(result.left_top().y, 8);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
    EXPECT_EQ(result.square(), 4);
}

TEST(RectangleIntersectionTest, NoIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 5, 5), Rectangle(10, 10, 15, 15),
                                    Rectangle(20, 20, 25, 25)};
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, PartialChainIntersection) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 10, 10),   // intersects with next
        Rectangle(5, 5, 15, 15),   // intersects with both
        Rectangle(20, 20, 25, 25)  // no intersection with first
    };
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, TouchingEdges) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 5, 5), Rectangle(5, 0, 10, 5)  // touches right edge
    };
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

// Tests for bounding box
TEST(BoundingBoxTest, EmptyVector) {
    std::vector<Rectangle> empty;
    Rectangle result = bounding_box(empty);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(BoundingBoxTest, SingleRectangle) {
    std::vector<Rectangle> rects = {Rectangle(5, 5, 15, 15)};
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 5);
    EXPECT_EQ(result.left_top().y, 5);
    EXPECT_EQ(result.right_bottom().x, 15);
    EXPECT_EQ(result.right_bottom().y, 15);
    EXPECT_EQ(result.square(), 100);
}

TEST(BoundingBoxTest, TwoRectanglesBounding) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(15, 15, 25, 25)};
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 25);
    EXPECT_EQ(result.right_bottom().y, 25);
    EXPECT_EQ(result.square(), 625);
}

TEST(BoundingBoxTest, ThreeRectanglesBounding) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 10, 5, 15),  // left side
        Rectangle(10, 0, 15, 5),  // top right
        Rectangle(8, 20, 12, 25)  // bottom middle
    };
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 15);
    EXPECT_EQ(result.right_bottom().y, 25);
    EXPECT_EQ(result.square(), 375);
}

TEST(BoundingBoxTest, DegenerateRectangles) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 0, 10),    // zero width
        Rectangle(5, 5, 15, 5),    // zero height
        Rectangle(10, 10, 20, 20)  // normal
    };
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 20);
    EXPECT_EQ(result.right_bottom().y, 20);
    EXPECT_EQ(result.square(), 400);
}

// Combined functionality tests
TEST(CombinedTest, IntersectionThenBoundingBox) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15),
                                    Rectangle(8, 8, 12, 12)};

    Rectangle intersected = intersection(rects);
    Rectangle bbox = bounding_box(rects);

    EXPECT_FALSE(intersected.is_degenerate());
    EXPECT_EQ(intersected.square(), 4);

    EXPECT_FALSE(bbox.is_degenerate());
    EXPECT_EQ(bbox.left_top().x, 0);
    EXPECT_EQ(bbox.left_top().y, 0);
    EXPECT_EQ(bbox.right_bottom().x, 15);
    EXPECT_EQ(bbox.right_bottom().y, 15);
    EXPECT_EQ(bbox.square(), 225);
}

TEST(CombinedTest, AllDegenerate) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 0, 10),  // zero width
        Rectangle(5, 5, 15, 5),  // zero height
        Rectangle(8, 8, 8, 8)    // point
    };

    Rectangle intersected = intersection(rects);
    Rectangle bbox = bounding_box(rects);

    EXPECT_TRUE(intersected.is_degenerate());
    EXPECT_EQ(intersected.square(), 0);

    EXPECT_FALSE(bbox.is_degenerate());
    EXPECT_EQ(bbox.left_top().x, 0);
    EXPECT_EQ(bbox.left_top().y, 0);
    EXPECT_EQ(bbox.right_bottom().x, 15);
    EXPECT_EQ(bbox.right_bottom().y, 10);
    EXPECT_EQ(bbox.square(), 150);
}