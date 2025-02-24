#include <iostream>

#include <timer.hpp>
#include <matrix.hpp>

#include <matrix_test.hpp>

int main(int argc, char** argv) {
    if (Core_Bond::Init() == false) {
        std::cout << "Failed to initialize OpenCL\n";
        return 1;
    }

    Timer timer;

    Matrix<double> x(3, 3, 2.0);
    double m = 2, c = 1;

    auto y = m * x + c;
    std::cout << "Bias: " << c << '\n';
    std::cout << "Gradient: " << m << '\n';
    std::cout << "Input:\n"; x.Print(2);
    std::cout << "Output:\n"; y.Print(2);
    std::cout << '\n';
}