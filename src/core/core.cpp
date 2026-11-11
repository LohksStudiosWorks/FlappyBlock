#include "core/core.hpp"

#include <format>

constexpr SDL::PointRaw c_default_app_size = {720, 1280};
constexpr SDL::PointRaw c_target_app_region = {144, 256};
constexpr SDL::WindowFlags c_default_app_flags = SDL::WINDOW_RESIZABLE | SDL::WINDOW_OPENGL;// | SDL::WINDOW_FULLSCREEN;

Core::Core(int argc, char *argv[]) {
    SDL::Log("Initializing video, audio, and events...");
    SDL::Init(SDL::INIT_VIDEO); //  | SDL::INIT_AUDIO | SDL::INIT_EVENTS

    SDL::Log("Initializing ttf...");
    SDL::TTF::Init();

    SDL::Log("Setup of screen and peripherals...");
    SDL::SetAppMetadata("FlappyBlock", "1.0", "com.lsw.flappyblock");
    SDL::CreateWindowAndRenderer("Flappy Block", c_default_app_size, c_default_app_flags, &m_sdl.window, &m_sdl.renderer);

    m_sdl.renderer.SetLogicalPresentation(c_target_app_region, SDL::RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_resources = std::make_shared<ResourceManager>(m_sdl.renderer);
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
    SDL::TTF::Quit();
}

SDL::AppResult Core::handle(SDL::Event& event) {
    switch(event.type) {
    case SDL::EVENT_QUIT:
        return SDL::APP_SUCCESS;
    case SDL::EVENT_KEY_UP:
        switch(event.key.scancode) {
        case SDL::SCANCODE_F11:
            SDL::Log("Would toggle fullscreen here");
            break;
        }
    }

    return m_game->event(event);
}

SDL::AppResult Core::draw() {
    m_sdl.renderer.RenderClear();

    m_game->draw();

    m_resources->draw_text(std::format("FPS: {:04.1f}", m_fps.get_avg_fps()), 0, 0);

    m_sdl.renderer.Present();
    m_fps.feed();
    return SDL::APP_CONTINUE;
}
