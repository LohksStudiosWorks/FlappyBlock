#include "utils/fps_counter.hpp"

#include <SDL3/SDL_timer.h>

#include <utility>
#include <algorithm>
#include <cmath>

void FpsCounter::feed() {
    const Uint64 current = SDL_GetTicksNS();
    const Uint64 delta = (current - std::exchange(m_last_feed, current));
    const double fps_inst = 1e9 / std::max(1e-15, static_cast<double>(delta));
    const double prop = fabs(m_fps_smooth);
    m_fps_smooth = (prop * m_fps_smooth + fps_inst) / (prop + 1.0);
}

double FpsCounter::get_avg_fps() const {
    return m_fps_smooth;
}