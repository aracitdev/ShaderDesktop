#include "Timer.h"

Timer::Timer()
{
    Restart();
}

void Timer::Restart(void)
{
    startTime=std::chrono::high_resolution_clock::now();
}

uint64_t Timer::GetTime(void)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}
