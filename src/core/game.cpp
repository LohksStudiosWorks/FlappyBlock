#include "core/game.hpp"

#include <cmath>

//constexpr double c_character_speed_x = 1.0;
constexpr double c_pipes_dist_each = 70.0;
constexpr double c_player_x_pos = 62.0;
constexpr double c_floor_y_pos = 200.0;
constexpr double c_character_gravity_y = 1.5;
constexpr double c_character_jump_accel_y = 8.0;

static float make_random_y();

GameLogic::GameLogic(std::shared_ptr<ResourceManager> resources, SDL_Renderer* renderer)
    : m_resources(resources), m_renderer_ref(renderer)
{}

void GameLogic::draw() {
    using TextId = ResourceManager::TextureId;

    switch(m_state) {
    case State::RANDOMIZE_MENU:
        m_sel_bg = SDL_GetTicksNS() % 2 == 0 ? TextId::BG_DARK : TextId::BG_LIGHT;
        m_state = State::MENU;
        [[fallthrough]];
    case State::MENU:
        draw_background();
        m_resources->draw_texture(TextId::MENU_LOGO, 27.0f, 36.0f);
        m_resources->draw_texture(TextId::BUTTON_START, 46.0f, 167.0f);
        //m_resources->draw_texture(TextId::BUTTON_SCORES, 75.0f, 167.0f);
        break;
    case State::HINT_START:
        m_background_offset = SDL_GetTicksNS();
        draw_background();
        draw_floor();
        m_resources->draw_texture(TextId::SIGN_GET_READY, 24.0f, 34.0f);
        m_resources->draw_texture(TextId::SIGN_TAP_HINT, 43.0f, 130.0f + 10.0f * sinf(SDL_GetTicksNS() * 2.5e-9f));
        break;
    case State::PLAYING:
        draw_background();
        draw_floor();
        draw_player();
        for(auto& i : m_pipes) {
            const float x = static_cast<float>(i.off_x.get(m_interp));
            m_resources->draw_texture(TextId::PIPE_GREEN_DOWN, x, i.get_upper());
            m_resources->draw_texture(TextId::PIPE_GREEN_UP,   x, i.get_lower());
        }

        {
            Uint64 m_points_cpy = m_points_usr;
            const int off_calc = snprintf(nullptr, 0, "%lu", m_points_cpy);
            const float off_x = (- 12.0f + 6.0f * static_cast<float>(off_calc));
            float curr_x = off_x;
            
            do {
                Uint64 rem = m_points_cpy % 10lu;
                m_points_cpy /= 10lu;
                switch(rem) {
                case 0: m_resources->draw_texture(TextId::NUMBER_0, 72.0f + curr_x, 20.0f); break;
                case 1: m_resources->draw_texture(TextId::NUMBER_1, 72.0f + curr_x, 20.0f); break;
                case 2: m_resources->draw_texture(TextId::NUMBER_2, 72.0f + curr_x, 20.0f); break;
                case 3: m_resources->draw_texture(TextId::NUMBER_3, 72.0f + curr_x, 20.0f); break;
                case 4: m_resources->draw_texture(TextId::NUMBER_4, 72.0f + curr_x, 20.0f); break;
                case 5: m_resources->draw_texture(TextId::NUMBER_5, 72.0f + curr_x, 20.0f); break;
                case 6: m_resources->draw_texture(TextId::NUMBER_6, 72.0f + curr_x, 20.0f); break;
                case 7: m_resources->draw_texture(TextId::NUMBER_7, 72.0f + curr_x, 20.0f); break;
                case 8: m_resources->draw_texture(TextId::NUMBER_8, 72.0f + curr_x, 20.0f); break;
                case 9: m_resources->draw_texture(TextId::NUMBER_9, 72.0f + curr_x, 20.0f); break;
                default: break;
                }
                curr_x -= 12.0f;
            } while(m_points_cpy > 0);
        }

        break;
    case State::END_PAUSE:
        m_background_offset = SDL_GetTicksNS();
        draw_background();
        draw_floor();
        m_resources->draw_texture(TextId::MENU_LOGO, 27.0f, 36.0f);
        m_resources->draw_texture(TextId::BUTTON_START, 46.0f, 167.0f);
        m_resources->draw_texture(TextId::END_FRAME_SCORE, 16.0f, 85.0f);

        if (m_points_usr >= 10) {
            if (m_points_usr < 20) {
                m_resources->draw_texture(TextId::MEDAL_BRONZE, 29.0f, 106.0f);
            } else if (m_points_usr < 30) {
                m_resources->draw_texture(TextId::MEDAL_SILVER, 29.0f, 106.0f);
            } else if (m_points_usr < 40) {
                m_resources->draw_texture(TextId::MEDAL_GOLD, 29.0f, 106.0f);
            } else {
                m_resources->draw_texture(TextId::MEDAL_PLATINUM, 29.0f, 106.0f);
            }
        }
        break;
    default:
        break;
    }
}

void GameLogic::tick() {
    using TextId = ResourceManager::TextureId;
    m_interp.set();

    switch(m_state) {
    case State::MENU:
        break;
    case State::PLAYING:
        m_player_accel_y -= c_character_gravity_y;
        m_player_y = m_player_y.curr - m_player_accel_y;

        for(auto& i : m_pipes) {
            if (i.tick())
                ++m_points_usr;
        }

        {
            const SDL_FPoint player_pos{c_player_x_pos, static_cast<float>(m_player_y.curr)};
            bool collided = false;

            for(auto& i : m_pipes) {
                const float x = static_cast<float>(i.off_x.get(m_interp));

                if (m_resources->check_texture_over_texture(
                    TextId::BIRD_0_FRAME_0, player_pos,
                    TextId::PIPE_GREEN_DOWN, {x, i.get_upper() - 5.0f}
                ) || m_resources->check_texture_over_texture(
                    TextId::BIRD_0_FRAME_0, player_pos,
                    TextId::PIPE_GREEN_UP, {x, i.get_lower() + 5.0f}
                )) {
                    collided = true;
                    break;
                }
            }

            //c_player_x_pos, m_player_y.curr
            if (collided || m_resources->check_texture_over_texture(
                TextId::BIRD_0_FRAME_0, player_pos,
                TextId::FLOOR, {0.0f, c_floor_y_pos + 5.0f}
            )) {
                m_state = State::END_PAUSE;
            }
        }
        break;
    default:
        break;
    }
}

Uint64 GameLogic::get_points() const {
    return m_points_usr;
}

SDL_AppResult GameLogic::event(SDL_Event& ev) {
    using TextId = ResourceManager::TextureId;

    switch(ev.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        SDL_ConvertEventToRenderCoordinates(m_renderer_ref, &ev);
        SDL_Log("Mouse click on %.2f %.2f", ev.button.x, ev.button.y);
        SDL_FPoint mouse_conv{ev.button.x, ev.button.y};

        switch(m_state) {
        case State::END_PAUSE:
            m_state = State::RANDOMIZE_MENU;
            [[fallthrough]];
        case State::MENU:
            if (m_resources->check_mouse_over(TextId::BUTTON_START, {46.0f, 167.0f}, mouse_conv)) {
                m_state = State::HINT_START;
            }

            break;
        case State::HINT_START:
            for(size_t p = 0; p < std::size(m_pipes); ++p)
                m_pipes[p].reset(p);

            m_player_y.curr = m_player_y.last = 130.0;
            m_player_accel_y = 0.0;
            m_points_usr = 0ULL;
            m_state = State::PLAYING;
            break;
        case State::PLAYING:
            m_player_accel_y = c_character_jump_accel_y;
            m_click_time = SDL_GetTicksNS();
            break;
        default:
            break;
        }
    }
        break;
    case SDL_EVENT_KEY_DOWN:

        switch(m_state) {
        case State::MENU:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_AC_BACK:
                [[fallthrough]];
            case SDL_SCANCODE_ESCAPE:
                return SDL_APP_SUCCESS;
            default:
                break;
            }            
            break;
        case State::HINT_START:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_AC_BACK:
                [[fallthrough]];
            case SDL_SCANCODE_ESCAPE:
                m_state = State::MENU;
            default:
                break;
            }
            break;
        case State::PLAYING:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_AC_BACK:
                [[fallthrough]];
            case SDL_SCANCODE_ESCAPE:
                m_state = State::END_PAUSE;
                break;
            case SDL_SCANCODE_SPACE:
                m_player_accel_y = c_character_jump_accel_y;
                m_click_time = SDL_GetTicksNS();
                break;
            default:
                break;
            }
            break;
        case State::END_PAUSE:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_AC_BACK:
                [[fallthrough]];
            case SDL_SCANCODE_ESCAPE:
                m_state = State::MENU;
            default:
                break;
            }
            break;
        default:
            break;
        }
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void GameLogic::draw_background() {
    const double x = - std::fmod(static_cast<double>(SDL_GetTicksNS() - m_background_offset) * 1.5e-8, 144.0);
    m_resources->draw_texture(m_sel_bg, static_cast<float>(x), 0.0f);
    m_resources->draw_texture(m_sel_bg, static_cast<float>(x) + 143.9f, 0.0f);
}

void GameLogic::draw_floor() {
    using TextId = ResourceManager::TextureId;

    const double x = - std::fmod(static_cast<double>(SDL_GetTicksNS() - m_background_offset) * 2.5e-8, 12.0);
    m_resources->draw_texture(TextId::FLOOR, static_cast<float>(x), c_floor_y_pos);
}

void GameLogic::draw_player() {
    using TextId = ResourceManager::TextureId;
    const double curr_delta_s = (SDL_GetTicksNS() - m_click_time) / 1e9;
    const double adv = curr_delta_s * 3.0 - 1.0;
    const double prop = std::max(0.0, std::min(adv, 1.0));
    const double ang = -35.0 + (70.0 * prop);

    const TextId texture_id = (
        adv < -0.6 ? TextId::BIRD_0_FRAME_2 : (
            adv < -0.3 ? TextId::BIRD_0_FRAME_1 : TextId::BIRD_0_FRAME_0
        )
    );

    m_resources->draw_texture_rotated(texture_id, c_player_x_pos, static_cast<float>(m_player_y.get(m_interp)), ang);
}


GameLogic::async_double::async_double(double s)
    : last(s), curr(s)
{}

double GameLogic::async_double::get(const Interpolator& interp) {
    const auto prop = interp.get_prop();
    return prop * curr + (1.0 - prop) * last;
}

void GameLogic::async_double::operator=(const double& nv) {
    last = curr;
    curr = nv;
}

bool GameLogic::pipe_pos::tick() {
    off_x = off_x.curr - 1.5;
    if (off_x.curr < -(0.5 * c_pipes_dist_each)) {
        off_x.last = (off_x.curr += c_pipes_dist_each * 3.0);
        y_pos = make_random_y();
    }
    const bool has_passed = off_x.curr < c_player_x_pos;
    const bool is_diff = has_passed != passed_middle;
    passed_middle = has_passed;
    return is_diff && has_passed;
}

void GameLogic::pipe_pos::reset(size_t idx) {
    off_x = 25.0f + static_cast<double>(idx + 3) * c_pipes_dist_each;
    passed_middle = false;
    y_pos = make_random_y();
}

float GameLogic::pipe_pos::get_upper() const {
    return y_pos - 150.0f;
}

float GameLogic::pipe_pos::get_lower() const {
    return y_pos + 80.0f;
}

float make_random_y() {
    const auto random = SDL_GetTicksNS() % 1000;
    const float prop = static_cast<float>(random) * 1e-3;
    return 20.0f + 85.0f * prop;
}