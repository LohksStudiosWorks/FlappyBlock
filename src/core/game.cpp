#include "core/game.hpp"

#include <cmath>

//constexpr double c_character_speed_x = 1.0;
constexpr double c_character_gravity_y = 1.5;
constexpr double c_character_jump_accel_y = 8.0;

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
        m_resources->draw_texture(TextId::BUTTON_START, 17.0f, 167.0f);
        m_resources->draw_texture(TextId::BUTTON_SCORES, 75.0f, 167.0f);
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
        break;
    default:
        break;
    }
}

void GameLogic::tick() {
    m_interp.set();

    switch(m_state) {
    case State::MENU:
        m_player_y.curr = m_player_y.last = 130.0;
        m_player_accel_y = 0.0;
        break;
    case State::PLAYING:
        m_player_accel_y -= c_character_gravity_y;
        m_player_y = m_player_y.curr - m_player_accel_y;
        break;
    default:
        break;
    }
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
        case State::MENU:
            if (m_resources->check_mouse_over(TextId::MENU_LOGO, {27.0f, 36.0f}, mouse_conv)) {
                m_sel_bg = m_sel_bg == TextId::BG_LIGHT ? TextId::BG_DARK : TextId::BG_LIGHT;
            }
            else if (m_resources->check_mouse_over(TextId::BUTTON_START, {17.0f, 167.0f}, mouse_conv)) {
                m_state = State::HINT_START;
            }
            else if (m_resources->check_mouse_over(TextId::BUTTON_SCORES, {75.0f, 167.0f}, mouse_conv)) {
                m_sel_bg = m_sel_bg == TextId::BG_LIGHT ? TextId::BG_DARK : TextId::BG_LIGHT;
            }

            break;
        case State::HINT_START:
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
    case SDL_EVENT_KEY_UP:

        switch(m_state) {
        case State::MENU:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                return SDL_APP_SUCCESS;
            default:
                break;
            }            
            break;
        case State::HINT_START:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                m_state = State::MENU;
            default:
                break;
            }
            break;
        case State::PLAYING:
            switch(ev.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                m_state = State::MENU;
                break;
            case SDL_SCANCODE_SPACE:
                m_player_accel_y = c_character_jump_accel_y;
                m_click_time = SDL_GetTicksNS();
                break;
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
    m_resources->draw_texture(TextId::FLOOR, static_cast<float>(x), 200.0f);
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

    m_resources->draw_texture_rotated(texture_id, 62.0f, m_player_y.get(m_interp), ang);
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