#pragma once

#include <chrono>
#include <thread>

class TimedBlock {
public:
    TimedBlock(std::chrono::milliseconds delta);
    ~TimedBlock();
private:
    const std::chrono::milliseconds m_delta;
    const decltype(std::chrono::high_resolution_clock::now()) m_beg;
};