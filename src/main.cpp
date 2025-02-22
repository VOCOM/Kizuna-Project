#include <iostream>
#include <chrono>

#include <matrix.hpp>

int main(int argc, char** argv) {
    if (Core_Bond::Init() == false) {
        std::cout << "Failed to initialize OpenCL\n";
        return 1;
    }

    std::chrono::time_point<std::chrono::steady_clock> now;
    std::chrono::duration<long long, std::nano> period;

    Matrix<int> m1(3, 3), m2(3, 3);
    for (int i = 0; i < m1.Size();i++) {
        m1[i] = i;
        m2[i] = i * 3;
    }

    // Warm-up Execution
    m1 + m2;

    // Identity Test
    std::cout << "Identity test\n";
    Matrix<int> I = Matrix<int>::I(5);
    std::cout << "Result:\n";
    I.Print(1);
    std::cout << "\n";

    // Transpose Test
    Matrix<int> t(2, 3);
    for (int i = 0; i < t.Size(); i++) t[i] = i;
    std::cout << "Transpose test\n";
    auto t1 = t.Transpose();
    std::cout << "Result:\n";
    t.Print(2);
    std::cout << "\n";
    t1.Print(2);
    std::cout << "\n";

    // Addition Test
    std::cout << "Additon test\n";
    now = std::chrono::steady_clock::now();
    auto m5 = m1 + m2;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "Result:\n";
    m5.Print();
    std::cout << "\n";

    // Subtraction Test
    std::cout << "Subtraction test\n";
    now = std::chrono::steady_clock::now();
    auto m6 = m1 - m2;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "Result:\n";
    m6.Print();
    std::cout << "\n";

    Matrix<int> m3(3, 5);
    for (int i = 0;i < m3.Size();i++) {
        m3[i] = 1;
        // m4[i] = 1;
    }

    Matrix<int> m4 = Matrix<int>::I(5);

    // Multiplication Test
    std::cout << "Multiplication test\n";
    now = std::chrono::steady_clock::now();
    auto m7 = 6 * m4;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "A:\n";
    m3.Print();
    std::cout << "\n";
    std::cout << "B:\n";
    m4.Print();
    std::cout << "\n";
    std::cout << "Result:\n";
    m7.Print();
    std::cout << "\n";

    // Inverse Test
    // Special Orthographic Test
}