#include <cmath>
#include <iostream>
#include <numbers>

class Triangle {
   private:
    double m_side1;
    double m_side2;
    double m_side3;

   public:
    Triangle(double s1, double s2, double s3) : m_side1(s1), m_side2(s2), m_side3(s3) {}

    double perimeter() const { return m_side1 + m_side2 + m_side3; }

    double area() const {
        double p = perimeter() / 2;
        return std::sqrt(p * (p - m_side1) * (p - m_side2) * (p - m_side3));
    }
};

class Square {
   private:
    double m_side;

   public:
    Square(double side) : m_side(side) {}

    double perimeter() const { return 4 * m_side; }

    double area() const { return m_side * m_side; }
};

class Circle {
   private:
    double m_radius;

   public:
    Circle(double radius) : m_radius(radius) {}

    double perimeter() const { return 2 * std::numbers::pi * m_radius; }

    double area() const { return std::numbers::pi * m_radius * m_radius; }
};

int main() {
    Triangle triangle(3, 4, 5);
    Square square(4);
    Circle circle(2.5);

    std::cout << "Triangle:" << '\n';
    std::cout << "Perimeter: " << triangle.perimeter() << '\n';
    std::cout << "Area: " << triangle.area() << "\n\n";

    std::cout << "Square:" << '\n';
    std::cout << "Perimeter: " << square.perimeter() << '\n';
    std::cout << "Area: " << square.area() << "\n\n";

    std::cout << "Circle:" << '\n';
    std::cout << "Perimeter: " << circle.perimeter() << '\n';
    std::cout << "Area: " << circle.area() << '\n';

    return 0;
}