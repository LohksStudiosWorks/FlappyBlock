#pragma once

#include <cstdint>

class Interpolator {
public:
    void set();
    double get_prop() const;
private:
    uint64_t m_last_set{}, m_last_delta{1};
};