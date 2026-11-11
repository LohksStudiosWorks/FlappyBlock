#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>
#include <SDL3pp/SDL3pp_image.h>
#include <SDL3pp/SDL3pp_ttf.h>

#include "core/core.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    Core* core = new Core(argc, argv);
    *appstate = (void*)core;
    return SDL::APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    return ((Core*)appstate)->handle(*event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    return ((Core*)appstate)->draw();
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    (void)result;    
    delete ((Core*)appstate);
}
