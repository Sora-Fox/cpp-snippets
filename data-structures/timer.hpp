#include <chrono>
#include <ratio>


class Timer
{
public:
    enum class Unit { NANO, MICRO, MILLI, SEC, MIN };
    void start() { this->begin = std::chrono::steady_clock::now(); }
    void stop() { this->end = std::chrono::steady_clock::now(); }
    double elapsed(Timer::Unit) const;
private:
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
};

double Timer::elapsed(Timer::Unit unit=Timer::Unit::MILLI) const
{
    switch (unit)
    {
        case Timer::Unit::NANO:
            return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(this->end - this->begin).count();
        case Timer::Unit::MICRO:
            return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(this->end - this->begin).count();
        case Timer::Unit::MILLI:
            return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(this->end - this->begin).count();
        case Timer::Unit::SEC:
            return std::chrono::duration_cast<std::chrono::duration<double>>(this->end - this->begin).count();
        case Timer::Unit::MIN:
            return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60>>>(this->end - this->begin).count();
        default:
            return 0.0;
    }
}
