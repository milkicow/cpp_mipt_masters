#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

class Shape {
   public:
    virtual ~Shape() = default;

    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Triangle : public Shape {
   private:
    double m_side1;
    double m_side2;
    double m_side3;

   public:
    Triangle(double s1, double s2, double s3) : m_side1(s1), m_side2(s2), m_side3(s3) {}

    double perimeter() const override final { return m_side1 + m_side2 + m_side3; }

    double area() const override final {
        double p = perimeter() / 2;
        return std::sqrt(p * (p - m_side1) * (p - m_side2) * (p - m_side3));
    }
};

class Square final : public Shape {
   private:
    double m_side;

   public:
    explicit Square(double side) : m_side(side) {}

    double perimeter() const override { return 4 * m_side; }
    double area() const override { return m_side * m_side; }
};

class Circle final : public Shape {
   private:
    double m_radius;

   public:
    explicit Circle(double radius) : m_radius(radius) {}

    double perimeter() const override { return 2 * std::numbers::pi * m_radius; }
    double area() const override { return std::numbers::pi * m_radius * m_radius; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Square>(4));
    shapes.push_back(std::make_unique<Circle>(2.5));

    for (const auto& shape : shapes) {
        std::cout << "Perimeter: " << shape->perimeter() << '\n';
        std::cout << "Area: " << shape->area() << "\n\n";
    }

    return 0;
}
