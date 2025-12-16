#ifndef RECTANGLE
#define RECTANGLE

#include <algorithm>
#include <vector>

namespace rectangle {

struct Point final {
    int x, y;
};

class Rectangle final {
   private:
    Point m_left_top, m_right_bottom;

   public:
    Rectangle(Point left_top, Point right_bottom)
        : m_left_top(left_top), m_right_bottom(right_bottom) {}
    Rectangle(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y)
        : m_left_top{left_top_x, left_top_y}, m_right_bottom{right_bottom_x, right_bottom_y} {}

    [[nodiscard]] const Point &left_top() const noexcept { return m_left_top; }
    [[nodiscard]] const Point &right_bottom() const noexcept { return m_right_bottom; }

    [[nodiscard]] unsigned width() const noexcept { return m_right_bottom.x - m_left_top.x; }
    [[nodiscard]] unsigned height() const noexcept { return m_right_bottom.y - m_left_top.y; }

    [[nodiscard]] bool is_degenerate() const noexcept { return width() == 0 || height() == 0; }

    int square() const noexcept {
        return (m_right_bottom.x - m_left_top.x) * (m_right_bottom.y - m_left_top.y);
    }
};

Rectangle intersection(const Rectangle &lhs, const Rectangle &rhs) {
    if (lhs.right_bottom().y <= rhs.left_top().y || lhs.right_bottom().x <= rhs.left_top().x ||
        lhs.left_top().y >= rhs.right_bottom().y || lhs.left_top().x >= rhs.right_bottom().x) {
        return Rectangle{0, 0, 0, 0};
    }

    int inter_left = std::max(lhs.left_top().x, rhs.left_top().x);
    int inter_top = std::max(lhs.left_top().y, rhs.left_top().y);
    int inter_right = std::min(lhs.right_bottom().x, rhs.right_bottom().x);
    int inter_bottom = std::min(lhs.right_bottom().y, rhs.right_bottom().y);

    return Rectangle(inter_left, inter_top, inter_right, inter_bottom);
}

Rectangle intersection(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty()) {
        return Rectangle(0, 0, 0, 0);
    }

    auto result = rectangles[0];

    for (std::size_t i = 1; i < rectangles.size(); ++i) {
        result = intersection(result, rectangles[i]);
        if (result.is_degenerate()) {
            break;
        }
    }

    return result;
}

Rectangle bounding_box(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty()) {
        return Rectangle(0, 0, 0, 0);
    }

    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();

    for (const auto &rect : rectangles) {
        min_x = std::min(min_x, rect.left_top().x);
        min_y = std::min(min_y, rect.left_top().y);
        max_x = std::max(max_x, rect.right_bottom().x);
        max_y = std::max(max_y, rect.right_bottom().y);
    }

    if (min_x > max_x || min_y > max_y) {
        return Rectangle(0, 0, 0, 0);
    }

    return Rectangle(min_x, min_y, max_x, max_y);
}

}  // namespace rectangle

#endif  // RECTANGLE