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

    // Addition Test
    now = std::chrono::steady_clock::now();
    Matrix<int> m5 = m1 + m2;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "Additon test\n";
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "Result:\n";
    m5.Print();

    // Subtraction Test
    now = std::chrono::steady_clock::now();
    Matrix<int> m6 = m1 - m2;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "Subtraction test\n";
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "Result:\n";
    m6.Print();

    Matrix<int> m3(3, 5), m4(5, 3);
    for (int i = 0;i < m3.Size();i++) {
        m3[i] = i;
        m4[i] = i + 1;
    }

    // Multiplication Test
    now = std::chrono::steady_clock::now();
    Matrix<int> m7 = m3 * m4;
    period = std::chrono::steady_clock::now() - now;
    std::cout << "Multiplication test\n";
    std::cout << "GPU: " << std::chrono::duration_cast<std::chrono::microseconds>(period).count() << "us\n";
    std::cout << "Result:\n";
    m7.Print();

    // Identity Test
    // Inverse Test
    // Transpose Test
    // Special Orthographic Test
}