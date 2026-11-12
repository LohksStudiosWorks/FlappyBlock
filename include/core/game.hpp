#pragma once

#include "textures/resource_manager.hpp"
#include "utils/interpolator.hpp"
#include "utils/score_manager.hpp"

#include <memory>

class GameLogic {
public:
    enum class State {
        RANDOMIZE_MENU,
        MENU,
        HINT_START,
        PLAYING,
        END_PAUSE
    };
    
    GameLogic(std::shared_ptr<ResourceManager> resources, SDL_Renderer* renderer);

    void draw();
    void tick();

    Uint64 get_points() const;

    SDL_AppResult event(SDL_Event& ev);
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

    struct pipe_pos {
        async_double off_x{0.0};
        float y_pos = 0.0f;
        bool passed_middle = false;

        bool tick();
        void reset(size_t idx);

        float get_upper() const;
        float get_lower() const;
    };

    std::shared_ptr<ResourceManager> m_resources;
    SDL_Renderer* const m_renderer_ref;
    Interpolator m_interp;
    ScoreManager m_score;
    pipe_pos m_pipes[3]{};
    State m_state = State::RANDOMIZE_MENU;
    ResourceManager::TextureId m_sel_bg = ResourceManager::TextureId::BG_DARK;
    Uint64 m_background_offset{};
    Uint64 m_click_time{};
    Uint64 m_points_usr{};
    //double m_bg_pos = 0.0;
    double m_player_accel_y = 0.0;
    async_double m_player_y{130.0};
};