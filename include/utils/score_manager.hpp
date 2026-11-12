#pragma once

#include <cstdint>

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();

    uint64_t get_score();
    void set_score_if_higher(uint64_t new_score);
private:
    uint64_t m_score{};
    bool m_changed = false;
};