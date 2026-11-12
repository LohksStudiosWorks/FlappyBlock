include(FetchContent)

message(STATUS "Fetching SDL3")

# --- Force static compilation ---
set(SDL_SHARED OFF CACHE BOOL "" FORCE)             # Disable dynamic SDL3 shared library (.dll/.so)
set(SDL_STATIC ON CACHE BOOL "" FORCE)              # Enable static SDL3 library (.lib/.a)

set(SDL3PP_FORCE_BUNDLED OFF CACHE BOOL "" FORCE)   # Uncomment this to make CMake to download SDL and SDL_image for you
set(SDL3PP_ENABLE_IMAGE ON CACHE BOOL "" FORCE)     # SDL_image; already on by default, set to OFF to disable
set(SDL3PP_ENABLE_MIXER ON CACHE BOOL "" FORCE)     # Enables SDL_mixer
set(SDL3PP_ENABLE_NET OFF CACHE BOOL "" FORCE)      # Enables SDL_net
set(SDL3PP_ENABLE_TTF ON CACHE BOOL "" FORCE)       # Enables SDL_ttf

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.20
)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
)
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        release-3.2.4
)
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-3.4.8
)
#FetchContent_Declare(
#    SDL_net
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
#    GIT_TAG        release-3.2.0
#)

FetchContent_MakeAvailable(SDL3 SDL3_ttf SDL3_mixer SDL3_image) # SDL_net

message(STATUS "Fetching of Fetching SDL3 done!")