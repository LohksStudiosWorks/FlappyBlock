#pragma once

#include <SDL3/SDL.h>

class FpsCounter {
public:
    void feed();

    double get_avg_fps() const;
public:
    double m_fps_smooth{};
    Uint64 m_last_feed{};
};