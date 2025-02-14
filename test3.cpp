#include <variant>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <random>
#include <cassert>
#include <cstddef>
#include <cstdint>

enum class ShapeTag { CIRCLE, SQUARE, TRIANGLE, STAR, ROUNDED_SQUARE };
using namespace std;
template <typename Derived>
class Shape {
protected:
    int data;
    ShapeTag tag;
public:

    Shape(int dat) : data(dat) {}

    int virtualCall() const {
        return static_cast<Derived*>(this)->virtualCall();
    }

    ShapeTag getTag() const { return tag;}
};

class Circle : public Shape<Circle> {
public:
    Circle(int dat) : Shape(dat) {
        tag = ShapeTag::CIRCLE;
    }

    int virtualCall() const {
        return data;
    }
};

class Square : public Shape<Square> {
public:
    Square(int dat) : Shape(dat) {
        tag = ShapeTag::SQUARE;
    }

    int virtualCall() const {
        return data;
    }
};

class RoundedSquare : public Square {
public:
    RoundedSquare(int dat) : Square(dat) {
        tag = ShapeTag::ROUNDED_SQUARE;
    }

    int virtualCall() const {
        return data;
    }
};

class Triangle : public Shape<Triangle> {
public:
    Triangle(int dat) : Shape(dat) {
        tag = ShapeTag::TRIANGLE;
    }

    int virtualCall() const {
        return data;
    }
};

class Star : public Shape<Star> {
public:
    Star(int dat) : Shape(dat) {
        tag = ShapeTag::STAR;
    }

    int virtualCall() const {
        return data;
    }
};

// Function to process each shape
void operateOn(const Circle& circle) {         // Start with the input value and perform a series of math-heavy operations
    double result = circle.virtualCall();

    // Basic arithmetic operations
    result = result * 3.14159265358979323846; // Multiply by Pi
    result = result / 2.71828182845904523536; // Divide by Euler's number
    result = result + 42; // Add 42
    result = result - 7;  // Subtract 7

    // Power and root operations
    result = std::pow(result, 2); // Square the result
    result = std::sqrt(result); // Take the square root

    // Logarithmic and exponential operations
    result = std::log(result); // Natural logarithm (ln)
    result = std::exp(result); // Exponentiation (e^x)

    // Trigonometric operations
    result = std::sin(result); // Sine of the result
    result = std::cos(result); // Cosine of the result
    result = std::tan(result); // Tangent of the result

    // Absolute value and rounding
    result = std::abs(result); // Absolute value
    result = std::round(result); // Round to nearest integer

    // Miscellaneous math functions
    result = std::fmod(result, 3); // Modulo 3
    result = std::ceil(result); // Round up to nearest integer
    result = std::floor(result); // Round down to nearest integer
    result = std::trunc(result); // Truncate to integer
        }
void operateOn(const Square& circle) {         // Start with the input value and perform a series of math-heavy operations
    double result = circle.virtualCall();

    // Basic arithmetic operations
    result = result * 3.14159265358979323846; // Multiply by Pi
    result = result / 2.71828182845904523536; // Divide by Euler's number
    result = result + 42; // Add 42
    result = result - 7;  // Subtract 7

    // Power and root operations
    result = std::pow(result, 2); // Square the result
    result = std::sqrt(result); // Take the square root

    // Logarithmic and exponential operations
    result = std::log(result); // Natural logarithm (ln)
    result = std::exp(result); // Exponentiation (e^x)

    // Trigonometric operations
    result = std::sin(result); // Sine of the result
    result = std::cos(result); // Cosine of the result
    result = std::tan(result); // Tangent of the result

    // Absolute value and rounding
    result = std::abs(result); // Absolute value
    result = std::round(result); // Round to nearest integer

    // Miscellaneous math functions
    result = std::fmod(result, 3); // Modulo 3
    result = std::ceil(result); // Round up to nearest integer
    result = std::floor(result); // Round down to nearest integer
    result = std::trunc(result); // Truncate to integer
        }
void operateOn(const Triangle& circle) {         // Start with the input value and perform a series of math-heavy operations
    double result = circle.virtualCall();

    // Basic arithmetic operations
    result = result * 3.14159265358979323846; // Multiply by Pi
    result = result / 2.71828182845904523536; // Divide by Euler's number
    result = result + 42; // Add 42
    result = result - 7;  // Subtract 7

    // Power and root operations
    result = std::pow(result, 2); // Square the result
    result = std::sqrt(result); // Take the square root

    // Logarithmic and exponential operations
    result = std::log(result); // Natural logarithm (ln)
    result = std::exp(result); // Exponentiation (e^x)

    // Trigonometric operations
    result = std::sin(result); // Sine of the result
    result = std::cos(result); // Cosine of the result
    result = std::tan(result); // Tangent of the result

    // Absolute value and rounding
    result = std::abs(result); // Absolute value
    result = std::round(result); // Round to nearest integer

    // Miscellaneous math functions
    result = std::fmod(result, 3); // Modulo 3
    result = std::ceil(result); // Round up to nearest integer
    result = std::floor(result); // Round down to nearest integer
    result = std::trunc(result); // Truncate to integer
        }
void operateOn(const Star& circle) {         // Start with the input value and perform a series of math-heavy operations
    double result = circle.virtualCall();

    // Basic arithmetic operations
    result = result * 3.14159265358979323846; // Multiply by Pi
    result = result / 2.71828182845904523536; // Divide by Euler's number
    result = result + 42; // Add 42
    result = result - 7;  // Subtract 7

    // Power and root operations
    result = std::pow(result, 2); // Square the result
    result = std::sqrt(result); // Take the square root

    // Logarithmic and exponential operations
    result = std::log(result); // Natural logarithm (ln)
    result = std::exp(result); // Exponentiation (e^x)

    // Trigonometric operations
    result = std::sin(result); // Sine of the result
    result = std::cos(result); // Cosine of the result
    result = std::tan(result); // Tangent of the result

    // Absolute value and rounding
    result = std::abs(result); // Absolute value
    result = std::round(result); // Round to nearest integer

    // Miscellaneous math functions
    result = std::fmod(result, 3); // Modulo 3
    result = std::ceil(result); // Round up to nearest integer
    result = std::floor(result); // Round down to nearest integer
    result = std::trunc(result); // Truncate to integer
        }
void operateOn(const RoundedSquare& circle) {         // Start with the input value and perform a series of math-heavy operations
    double result = circle.virtualCall();

    // Basic arithmetic operations
    result = result * 3.14159265358979323846; // Multiply by Pi
    result = result / 2.71828182845904523536; // Divide by Euler's number
    result = result + 42; // Add 42
    result = result - 7;  // Subtract 7

    // Power and root operations
    result = std::pow(result, 2); // Square the result
    result = std::sqrt(result); // Take the square root

    // Logarithmic and exponential operations
    result = std::log(result); // Natural logarithm (ln)
    result = std::exp(result); // Exponentiation (e^x)

    // Trigonometric operations
    result = std::sin(result); // Sine of the result
    result = std::cos(result); // Cosine of the result
    result = std::tan(result); // Tangent of the result

    // Absolute value and rounding
    result = std::abs(result); // Absolute value
    result = std::round(result); // Round to nearest integer

    // Miscellaneous math functions
    result = std::fmod(result, 3); // Modulo 3
    result = std::ceil(result); // Round up to nearest integer
    result = std::floor(result); // Round down to nearest integer
    result = std::trunc(result); // Truncate to integer
        }

using ShapeVariant = std::variant<Circle, Square, Triangle, Star, RoundedSquare>;

void dispatchFunction(ShapeTag tag, const ShapeVariant& s) {
    switch (tag) {
        using t = ShapeTag;
        case t::CIRCLE:
            operateOn(std::get<Circle>(s));
            break;
        case t::SQUARE:
            operateOn(std::get<Square>(s));
            break;
        case t::TRIANGLE:
            operateOn(std::get<Triangle>(s));
            break;
        case t::STAR:
            operateOn(std::get<Star>(s));
            break;
        case t::ROUNDED_SQUARE:
            operateOn(std::get<RoundedSquare>(s));
            break;
        default:
            __assume(false);
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<ShapeVariant> shapes;
    shapes.reserve(1'000'000'000);

    for (int i = 0; i <= 1'000'000'000; i++) {
        shapes.emplace_back(Circle(i));
        shapes.emplace_back(Square(i));
        shapes.emplace_back(Star(i));
        shapes.emplace_back(Triangle(i));
        shapes.emplace_back(RoundedSquare(i));
    }

    for (const auto& shape : shapes) {
        std::visit([&](auto&& _){
            operateOn(_);
        }, shape);
    }

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
}