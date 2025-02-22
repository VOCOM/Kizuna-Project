#include <iostream>
#include <chrono>

#include <matrix.hpp>

class Timer {
public:
    using Clock = std::chrono::steady_clock;

    inline void Start() { _start = Clock::now(); }
    inline void Stop() { period = Clock::now() - _start; }

    inline long long DurationNS() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(); }
    inline long long DurationUS() const { return std::chrono::duration_cast<std::chrono::microseconds>(period).count(); }
    inline long long DurationMS() const { return std::chrono::duration_cast<std::chrono::milliseconds>(period).count(); }

private:

    std::chrono::time_point<Clock> _start;
    std::chrono::duration<long long, std::nano> period;
};

int main(int argc, char** argv) {
    if (Core_Bond::Init() == false) {
        std::cout << "Failed to initialize OpenCL\n";
        return 1;
    }

    Timer timer;

    Matrix m1(3, 3, 2.2), m2(3, 3, 5.5), m3(2, 2, 2.0);
    m3[0] = 4;
    m3[1] = 7;
    m3[2] = 2;
    m3[3] = 6;

    // Warm-up Execution
    m1 + m2;
    m1 - m2;

    // Inverse Test
    std::cout << "\nInverse test\n";
    timer.Start();
    auto m5 = m3 * m3.Inverse();
    timer.Stop();
    std::cout << "GPU: " << timer.DurationUS() << "us\n";
    std::cout << "\nResult:\n";
    m5.Print(4);

    std::cout << "\nDeterminant: " << m5.Determinant();
    std::cout << "\nOrthoganal: " << (m5.IsOrthoganal() ? "Yes" : "No");

    // Special Orthographic Test
}