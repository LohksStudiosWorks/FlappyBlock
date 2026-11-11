#include "utils/discord_sdk.hpp"
#include "utils/fps_counter.hpp"
#include "utils/timed_block.hpp"
#include "textures/resource_manager.hpp"
#include "core/game.hpp"

#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>
#include <SDL3pp/SDL3pp_image.h>
#include <SDL3pp/SDL3pp_ttf.h>

#include <thread>
#include <atomic>

class Core {
public:
    Core(int argc, char *argv[]);
    ~Core();

    SDL::AppResult handle(SDL::Event& event);
    SDL::AppResult draw();
private:
    struct sdl_data {
        SDL::Window window{};
        SDL::Renderer renderer{};
    };

    std::shared_ptr<ResourceManager> m_resources;
    std::unique_ptr<GameLogic> m_game;
    DiscordManager m_discord{};
    FpsCounter m_fps;
    sdl_data m_sdl;
    std::jthread m_game_tick_thread;
};