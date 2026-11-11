#pragma once

#include <SDL3pp/SDL3pp.h>

class FpsCounter {
public:
    void feed();

    double get_avg_fps() const;
public:
    double m_fps_smooth{};
    SDL::Uint64 m_last_feed{};
};