#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/**************************************************************************************************
* \file Timer.h
* \author Sean C
* \brief Defines the timer class.
* \version 1.0
* Defines an stl (chrono) based implementation of a timer.
***************************************************************************************************/

#include <chrono>



/**************************************************************************************************
* \class Timer
* \brief A timer class designed to get the time since the timer has last been started.
* A timer class designed to get the time since the timer has last been started.
* Uses the STL implementation (chrono).
***************************************************************************************************/
class Timer
{
public:

/**************************************************************************************************
* \fn Timer()
* \brief Sets the startTime to the current time
* Sets up the timer by setting the startTime to the current time, used to get duration.
***************************************************************************************************/
    Timer();

/**************************************************************************************************
* \fn uint64_t GetTime()
* \brief Gets the time elapsed since the start time
* Gets the time elapsed since the current start time (in Ms)
* \return The time elapsed since the last restart in ms
***************************************************************************************************/
    uint64_t GetTime(void);

/**************************************************************************************************
* \fn void Restart()
* \brief Sets the startTime to the current time
* Restarts the timer by setting the start time to the current time, used to get duration.
***************************************************************************************************/
    void Restart(void);


private:
    std::chrono::high_resolution_clock::time_point startTime; /**< Last time the timer was restarted */
};


#endif // TIMER_H_INCLUDED
