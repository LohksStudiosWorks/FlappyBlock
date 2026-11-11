#include "utils/timed_block.hpp"

TimedBlock::TimedBlock(std::chrono::milliseconds delta) 
    : m_delta(delta), m_beg(std::chrono::high_resolution_clock::now())
{}

TimedBlock::~TimedBlock() {
    const auto elapsed = std::chrono::high_resolution_clock::now() - m_beg;
    
    if (elapsed < m_delta) {
        std::this_thread::sleep_for(m_delta - elapsed);
    }
}
