#include "utils/fps_counter.hpp"
#include "utils/timed_block.hpp"
#include "textures/resource_manager.hpp"
#include "core/game.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <thread>
#include <atomic>

class Core {
public:
    Core(int argc, char *argv[]);
    ~Core();

    SDL_AppResult handle(SDL_Event& event);
    SDL_AppResult draw();
private:
    struct sdl_data {
        SDL_Window* window{};
        SDL_Renderer* renderer{};
    };

    std::shared_ptr<ResourceManager> m_resources;
    std::unique_ptr<GameLogic> m_game;
    FpsCounter m_fps;
    sdl_data m_sdl;
    std::thread m_game_tick_thread;
    std::atomic_bool m_game_thread_running;
};