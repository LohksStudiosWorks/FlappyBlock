include(FetchContent)

message(STATUS "Declaring SDL3 and dependencies...")

# Declare SDL3
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.20
)
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-3.4.8
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
#FetchContent_Declare(
#    SDL_net
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
#    GIT_TAG        release-3.2.0
#)

message(STATUS "Setting SDL3 flags...")

if(ANDROID)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
else()
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
endif()

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)

set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIFF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)

message(STATUS "Making SDL3 available...")
FetchContent_MakeAvailable(SDL3)
message(STATUS "Making SDL3_image available...")
FetchContent_MakeAvailable(SDL3_image)
message(STATUS "Making SDL3_ttf available...")
FetchContent_MakeAvailable(SDL3_ttf)
message(STATUS "Making SDL3_mixer available...")
FetchContent_MakeAvailable(SDL3_mixer)
#message(STATUS "Making SDL_net available...")
#FetchContent_MakeAvailable(SDL_net)

message(STATUS "SDL3 is complete.")