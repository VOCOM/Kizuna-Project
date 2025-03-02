#ifndef TIMER
#define TIMER

#include <chrono>

class Timer {
public:
    using Clock = std::chrono::steady_clock;

    inline void Start() { _start = Clock::now(); }
    inline void Stop() { period = Clock::now() - _start; }

    inline long long DurationNS() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(period).count(); }
    inline long long DurationUS() const { return std::chrono::duration_cast<std::chrono::microseconds>(period).count(); }
    inline long long DurationMS() const { return std::chrono::duration_cast<std::chrono::milliseconds>(period).count(); }
    inline long long DurationS() const { return std::chrono::duration_cast<std::chrono::seconds>(period).count(); }

private:

    std::chrono::time_point<Clock> _start;
    std::chrono::duration<long long, std::nano> period;
};


#endif /* TIMER */
