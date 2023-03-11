#ifndef KEDIXA_TIMER_H
#define KEDIXA_TIMER_H

#include <chrono>
#include <cstdint>

namespace kedixa {

class timer {
    constexpr static int64_t NANO_PER_SEC = 1000000000;

public:
    static int64_t monotonic_nsec() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }

    static int64_t monotonic_usec() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }

    static int64_t monotonic_msec() {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }

    // return nano seconds cost by calling f(args...);
    template<typename F, typename... Args>
    static int64_t timeit(F&& f, Args&&... args) {
        timer t;
        std::forward<F>(f)(std::forward<Args>(args)...);
        t.stop();
        return t.as_nsec();
    }

public:
    timer() : begin_nsec(monotonic_nsec()), total_nsec(0) { }
    timer(const timer &) = default;
    timer& operator= (const timer &) = default;

    void start() {
        begin_nsec = monotonic_nsec();
    }

    void pause() {
        int64_t end_nsec = monotonic_nsec();
        total_nsec += (end_nsec - begin_nsec);
    }

    void reset() {
        begin_nsec = monotonic_nsec();
        total_nsec = 0;
    }

    // return total seconds with type double
    double stop() {
        int64_t end_nsec = monotonic_nsec();
        total_nsec += (end_nsec - begin_nsec);
        return double(total_nsec) / NANO_PER_SEC;
    }

    int64_t as_nsec() {
        return total_nsec;
    }

    int64_t as_usec() {
        return total_nsec / 1000;
    }

    int64_t as_msec() {
        return total_nsec / 1000000;
    }

private:
    int64_t begin_nsec;
    int64_t total_nsec;
};

} // namespace kedixa

#endif // KEDIXA_TIMER_H
