#pragma once

#include "textures/resource_manager.hpp"
#include "utils/interpolator.hpp"

class GameLogic {
public:
    enum class State {
        RANDOMIZE_MENU,
        MENU,
        HINT_START,
        PLAYING,
        PAUSE
    };
    
    GameLogic(std::shared_ptr<ResourceManager> resources, SDL::Renderer& renderer);

    void draw();
    void tick();

    SDL::AppResult event(SDL::Event& ev);
private:
    void draw_background();
    void draw_floor();
    void draw_player();

    struct async_double { 
        double last, curr;
        async_double(double s);
        double get(const Interpolator& interp);
        void operator=(const double& nv);
    };

    std::shared_ptr<ResourceManager> m_resources;
    SDL::Renderer& m_renderer_ref;
    Interpolator m_interp;
    State m_state = State::RANDOMIZE_MENU;
    ResourceManager::TextureId m_sel_bg = ResourceManager::TextureId::BG_DARK;
    SDL::Uint64 m_background_offset{};
    SDL::Uint64 m_click_time{};
    double m_bg_pos = 0.0;
    double m_player_accel_y = 0.0;
    async_double m_player_y{130.0};
};