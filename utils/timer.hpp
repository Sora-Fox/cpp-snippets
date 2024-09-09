#ifndef FTL_TIMER_HPP
#define FTL_TIMER_HPP

#include <chrono>

namespace ftl {

class Timer {
public:
    enum class Unit { NANO, MICRO, MILLI, SEC, MIN };
    Timer() { start(); }
    void start();
    void stop();
    double elapsed(Timer::Unit) const;

private:
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    bool stopped;
};

void Timer::start() {
    stopped = false;
    begin = std::chrono::steady_clock::now();
}

void Timer::stop() {
    end = std::chrono::steady_clock::now();
    stopped = true;
}

double Timer::elapsed(Timer::Unit unit = Timer::Unit::MILLI) const {
    using std::chrono::duration;
    using std::chrono::duration_cast;

    std::chrono::time_point<std::chrono::steady_clock> end;
    end = stopped ? end : std::chrono::steady_clock::now();

    switch (unit) {
    case Timer::Unit::NANO:
        return duration_cast<duration<double, std::nano>>(end - begin).count();
    case Timer::Unit::MICRO:
        return duration_cast<duration<double, std::micro>>(end - begin).count();
    case Timer::Unit::MILLI:
        return duration_cast<duration<double, std::milli>>(end - begin).count();
    case Timer::Unit::SEC:
        return duration_cast<duration<double>>(end - begin).count();
    case Timer::Unit::MIN:
        return duration_cast<duration<double, std::ratio<60>>>(end - begin)
            .count();
    default: return 0.0;
    }
}

}  // namespace ftl

#endif  // FTL_TIMER_HPP
