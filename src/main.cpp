#include <iostream>

#include <timer.hpp>
#include <matrix.hpp>

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

    Matrix<double> A(3, 3);
    for (auto& i : A) i = std::rand() % 10;
    auto A_inv = A.Inverse();

    std::cout << "A:\n"; A.Print(1);
    std::cout << "Cofactor:\n"; A.Cofactor().Print(5);
    std::cout << "Determinant: " << A.Determinant() << "\n";
    std::cout << "Inverse:\n"; A_inv.Print(4);
    std::cout << "Proof:\n"; (A * A_inv).Print(3);
}