#include "utils/interpolator.hpp"

#include <SDL3/SDL_timer.h>

#include <utility>
#include <algorithm>

void Interpolator::set() {
    const uint64_t current = SDL_GetTicksNS();
    m_last_delta = std::max(1UL, current - std::exchange(m_last_set, current));
}

double Interpolator::get_prop() const {
    const uint64_t delta = SDL_GetTicksNS() - m_last_set;
    const long double prop = static_cast<long double>(delta) / static_cast<long double>(m_last_delta);
    return static_cast<double>(prop < 0.0L ? 0.0L : (prop > 1.0L ? 1.0L : prop));
}
