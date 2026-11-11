include(FetchContent)

message(STATUS "Fetching SDL3pp")

# --- Force static compilation ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)             # Disable dynamic SDL3 shared library (.dll/.so)
set(SDL_STATIC ON CACHE BOOL "" FORCE)              # Enable static SDL3 library (.lib/.a)

set(SDL3PP_FORCE_BUNDLED ON CACHE BOOL "" FORCE)    # Uncomment this to make CMake to download SDL and SDL_image for you
set(SDL3PP_ENABLE_IMAGE ON CACHE BOOL "" FORCE)     # SDL_image; already on by default, set to OFF to disable
set(SDL3PP_ENABLE_MIXER ON CACHE BOOL "" FORCE)     # Enables SDL_mixer
set(SDL3PP_ENABLE_NET ON CACHE BOOL "" FORCE)       # Enables SDL_net
set(SDL3PP_ENABLE_TTF ON CACHE BOOL "" FORCE)       # Enables SDL_ttf

FetchContent_Declare(
    SDL3ppExternal
    GIT_REPOSITORY https://github.com/talesm/SDL3pp.git
    GIT_TAG        0.11.5
)

FetchContent_MakeAvailable(SDL3ppExternal)

message(STATUS "Fetching of Fetching SDL3pp done!")