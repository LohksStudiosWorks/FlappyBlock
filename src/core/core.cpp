#include "core/core.hpp"

#include <format>

constexpr SDL_Point c_default_app_size = {720, 1280};
constexpr SDL_Point c_target_app_region = {144, 256};
constexpr SDL_WindowFlags c_default_app_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;// | SDL_WINDOW_FULLSCREEN;

Core::Core(int argc, char *argv[]) {
    SDL_Log("Initializing video, audio, and events...");
    SDL_Init(SDL_INIT_VIDEO); //  | SDL_INIT_AUDIO | SDL_INIT_EVENTS

    SDL_Log("Initializing ttf...");
    TTF_Init();

    SDL_Log("Setup of screen and peripherals...");
    SDL_SetAppMetadata("FlappyBlock", "1.0", "com.lsw.flappyblock");
    SDL_CreateWindowAndRenderer("Flappy Block", c_default_app_size.x, c_default_app_size.y, c_default_app_flags, &m_sdl.window, &m_sdl.renderer);
    SDL_SetRenderLogicalPresentation(m_sdl.renderer, c_target_app_region.x, c_target_app_region.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_resources = std::make_shared<ResourceManager>(m_sdl.renderer);
    SDL_SetWindowIcon(m_sdl.window, m_resources->get_icon());

    m_game = std::make_unique<GameLogic>(m_resources, m_sdl.renderer);
    m_game_tick_thread = std::jthread([this](std::stop_token running) {
        while(!running.stop_requested()) {
            TimedBlock block(std::chrono::milliseconds(50));
            m_game->tick();
        }
    });
}

Core::~Core() {
    m_game_tick_thread.request_stop();
    m_game_tick_thread.join();
    m_game.reset();
    m_resources.reset();
    SDL_DestroyRenderer(m_sdl.renderer);
    SDL_DestroyWindow(m_sdl.window);
    TTF_Quit();
}

SDL_AppResult Core::handle(SDL_Event& event) {
    switch(event.type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_UP:
        switch(event.key.scancode) {
        case SDL_SCANCODE_F11:
            SDL_SetWindowFullscreen(m_sdl.window, !(SDL_GetWindowFlags(m_sdl.window) & SDL_WINDOW_FULLSCREEN));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return m_game->event(event);
}

SDL_AppResult Core::draw() {
    SDL_RenderClear(m_sdl.renderer);

    m_game->draw();

    m_resources->draw_text(std::format("FPS: {:04.1f}", m_fps.get_avg_fps()), 0, 0);

    SDL_RenderPresent(m_sdl.renderer);
    m_fps.feed();
    return SDL_APP_CONTINUE;
}
