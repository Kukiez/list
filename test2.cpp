#include <iostream>

using namespace std;
#include <stdexcept>
#include <chrono>
#include <vector>
#include <random>
class Shape {
protected:

int data;

public:
enum Tag {
    CIRCLE, SQUARE
} tag;

    Shape(int dat) : data(dat) {}

    int virtualCall() {
        return data;
    }

    virtual void operateOn() {
        cout << this->virtualCall() << std::endl;
    }
};

class Circle : public Shape {
public:
    Circle(int dat) : Shape(dat) {
        tag = CIRCLE;
    }

    int virtualCall() const {
        return data;
    }

    void operateOn() override {
        // Start with the input value and perform a series of math-heavy operations
        double result = virtualCall();

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
};

class Square : public Shape {
public:
    Square(int dat) : Shape(dat) {
        tag = SQUARE;
    }

    int virtualCall() const {
        return data;
    }

    void operateOn() override {
    // Start with the input value and perform a series of math-heavy operations
    double result = virtualCall();

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
};

class Triangle : public Shape {
    public:
        Triangle(int dat) : Shape(dat) {
            tag = SQUARE;
        }
    
        int virtualCall() const {
            return data;
        }
    
        void operateOn() override {
    // Start with the input value and perform a series of math-heavy operations
    double result = virtualCall();

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
    };
    class Star : public Shape {
        public:
            Star(int dat) : Shape(dat) {
                tag = SQUARE;
            }
        
            int virtualCall() const {
                return data;
            }
        
            void operateOn() override {
        // Start with the input value and perform a series of math-heavy operations
        double result = virtualCall();
    
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
        };
        class RoundedSquare : public Square {
            public:
                RoundedSquare(int dat) : Square(dat) {
                    tag = SQUARE;
                }
            
                int virtualCall() const {
                    return data;
                }
            
                void operateOn() override {
            // Start with the input value and perform a series of math-heavy operations
            double result = virtualCall();
        
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
            };
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
int main() {
    auto start = chrono::high_resolution_clock::now();

    vector<Shape*> shapes;
    shapes.reserve(100'000);
    RandomSource rand(0, 100);

    for (int i = 0; i <= 100'000; i++) {
        shapes.emplace_back(new Circle(i));
        shapes.emplace_back(new Square(i));
        shapes.emplace_back(new Star(i));
        shapes.emplace_back(new Triangle(i));
        shapes.emplace_back(new RoundedSquare(i));
    }

    for (auto& shape : shapes) {
        shape->operateOn();
    }
    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
}