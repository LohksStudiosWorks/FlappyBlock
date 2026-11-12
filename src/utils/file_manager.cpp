#include "utils/file_manager.hpp"

#include <SDL3/SDL.h>

std::string get_app_files_path(std::string for_file) {
    char* pref_path = SDL_GetPrefPath("LohksStudiosWorks", "FlappyBlock");
    if (!pref_path) {
        SDL_Log("Failed to get preference path: %s", SDL_GetError());
        return for_file;
    }

    std::string path = std::string(pref_path) + for_file;
    SDL_free(pref_path);
    return path;
}