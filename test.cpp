#include <iostream>

using namespace std;
#include <stdexcept>
#include <chrono>
#include <vector>
#include <random>

struct RandomSource {
    const int min, max;

    std::mt19937 gen;

    explicit RandomSource(const int min = 0, const int max = 100):
     min(min),
     max(max),
     gen(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()))
    {}

    int rand() {
        return uniform_int_distribution<int>(min, max)(gen);
    }

    float randf() {
        return uniform_real_distribution<float>(static_cast<float>(min), static_cast<float>(max))(gen);
    }
};

enum class ShapeTag {
    CIRCLE, SQUARE, TRIANGLE, STAR, ROUNDED_SQUARE
};

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

void operateOn(const Square& square) {
        // Start with the input value and perform a series of math-heavy operations
        double result = square.virtualCall();

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

void operateOn(const Circle& circle) {
    // Start with the input value and perform a series of math-heavy operations
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

void operateOn(const Triangle& triangle) {
    // Start with the input value and perform a series of math-heavy operations
    double result = triangle.virtualCall();

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

void operateOn(const Star& triangle) {
    // Start with the input value and perform a series of math-heavy operations
    double result = triangle.virtualCall();

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
void operateOn(const RoundedSquare& triangle) {
    // Start with the input value and perform a series of math-heavy operations
    double result = triangle.virtualCall();

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

class ShapeVariant {
private:
    void* variant{};

    ShapeTag tag;
public:
    template <typename T>
    ShapeVariant(T* shape) {
        variant = shape;

        if constexpr (is_same_v<decay_t<T>, Circle>) {
            tag = ShapeTag::CIRCLE;
        } else if constexpr (is_same_v<decay_t<T>, Square>) {
            tag = ShapeTag::SQUARE;
        } else if constexpr (is_same_v<decay_t<T>, Triangle>) {
            tag = ShapeTag::TRIANGLE;
        } else if constexpr (is_same_v<decay_t<T>, Star>) {
            tag = ShapeTag::STAR;
        } else if constexpr (is_same_v<decay_t<T>, RoundedSquare>) {
            tag = ShapeTag::ROUNDED_SQUARE;
        }
    }

    void* getActiveShape() const {
        return variant;
    }

    ShapeTag getTag() const {
        return tag;
    }
};

union VirtualTable {
    void (*circleCall)(const Circle&);
    void (*squareCall)(const Square&);
    void (*triangleCall)(const Triangle&);
    void (*starCall)(const Star&);
    void (*roundedsqCall)(const RoundedSquare&);
};

void dispatchFunction(ShapeTag tag, const void* arg) {
    VirtualTable function;

    switch (tag) {
        case ShapeTag::CIRCLE:
            function.circleCall = &operateOn;
            function.circleCall(*static_cast<const Circle*>(arg));
            break;

        case ShapeTag::SQUARE:
            function.squareCall = &operateOn;
            function.squareCall(*static_cast<const Square*>(arg));
            break;
        case ShapeTag::TRIANGLE:
            function.triangleCall = &operateOn;
            function.triangleCall(*static_cast<const Triangle*>(arg));
            break;
        case ShapeTag::STAR:
            function.starCall = &operateOn;
            function.starCall(*static_cast<const Star*>(arg));
            break;
            case ShapeTag::ROUNDED_SQUARE:
            function.roundedsqCall = &operateOn;
            function.roundedsqCall(*static_cast<const RoundedSquare*>(arg));
            break;
    }
}

struct _VirtualTable {
    void (*_VTable)(const void*);
};

void _dispatchFunction(ShapeTag tag, const void* arg) {
    _VirtualTable function;

    switch (tag) { 
        case ShapeTag::CIRCLE:
            function._VTable = reinterpret_cast<void(*)(const void*)>(*static_cast<void(*)(const Circle&)>(&operateOn));
            break;
        case ShapeTag::SQUARE:
            function._VTable = reinterpret_cast<void(*)(const void*)>(*static_cast<void(*)(const Square&)>(&operateOn));
            break;
        case ShapeTag::TRIANGLE:
            function._VTable = reinterpret_cast<void(*)(const void*)>(*static_cast<void(*)(const Triangle&)>(&operateOn));
            break;
        case ShapeTag::STAR:
            function._VTable = reinterpret_cast<void(*)(const void*)>(*static_cast<void(*)(const Star&)>(&operateOn));
            break;
        case ShapeTag::ROUNDED_SQUARE:
            function._VTable = reinterpret_cast<void(*)(const void*)>(*static_cast<void(*)(const RoundedSquare&)>(&operateOn));
            break;
    }

    function._VTable(arg);
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<ShapeVariant> shapes;
    shapes.reserve(100'000);
    RandomSource rand(0, 100);

    for (int i = 0; i <= 100'000; i++) {
        shapes.emplace_back(new Circle(i));
        shapes.emplace_back(new Square(i));
        shapes.emplace_back(new Star(i));
        shapes.emplace_back(new Triangle(i));
        shapes.emplace_back(new RoundedSquare(i));
    }

    for (const auto& shape : shapes) {
        _dispatchFunction(shape.getTag(), shape.getActiveShape());
    }

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
}