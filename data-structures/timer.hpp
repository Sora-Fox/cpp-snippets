#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <ratio>


class Timer
{
public:
    enum class Unit { NANO, MICRO, MILLI, SEC, MIN };
    Timer() { this->start(); }
    void start(); 
    void stop(); 
    double elapsed(Timer::Unit) const;

private:
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    bool stopped;
};

void Timer::start()
{
    this->stopped = false;
    this->begin = std::chrono::steady_clock::now();
}

void Timer::stop()
{
    this->end = std::chrono::steady_clock::now(); 
    this->stopped = true;
}

double Timer::elapsed(Timer::Unit unit=Timer::Unit::MILLI) const
{
    std::chrono::time_point<std::chrono::steady_clock> end;
    end = this->stopped ? this->end : std::chrono::steady_clock::now();

    switch (unit)
    {
        case Timer::Unit::NANO:
            return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(end - this->begin).count();
        case Timer::Unit::MICRO:
            return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(end - this->begin).count();
        case Timer::Unit::MILLI:
            return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - this->begin).count();
        case Timer::Unit::SEC:
            return std::chrono::duration_cast<std::chrono::duration<double>>(end - this->begin).count();
        case Timer::Unit::MIN:
            return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60>>>(end - this->begin).count();
        default:
            return 0.0;
    }
}


#endif // TIMER_HPP
