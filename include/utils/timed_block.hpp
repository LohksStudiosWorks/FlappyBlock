#pragma once

#include <chrono>
#include <thread>

class TimedBlock {
public:
    TimedBlock(std::chrono::milliseconds delta);
    ~TimedBlock();
private:
    const std::chrono::milliseconds m_delta;
    const std::chrono::system_clock::time_point m_beg;
};