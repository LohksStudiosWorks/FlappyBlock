# Setup dependency

include(FetchContent)

message(STATUS "Fetching SDL3_plus")

FetchContent_Declare(
    sdl3_plus
    GIT_REPOSITORY  https://github.com/LohksStudiosWorks/SDL3_plus
    GIT_TAG         v1.0.2
)
FetchContent_MakeAvailable(sdl3_plus)

message(STATUS "Fetching of SDL3_plus done!")