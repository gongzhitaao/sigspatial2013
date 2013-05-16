#ifndef _TESTCLOCK_H_
#define _TESTCLOCK_H_

#pragma once

#include <chrono>

class Clock
{
public:
    Clock() : start_(clock::now()) {}
    void reset() { start_ = clock::now(); }
    float elapsed() const { return (clock::now() - start_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock;
    std::chrono::time_point<clock> start_;
};

#endif /* _TESTCLOCK_H_ */
