cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 1.3.0
    DESCRIPTION "FlappyBlock is a remake of a popular game made in C/C++ with SDL3"
    LANGUAGES CXX
)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Od" CACHE STRING "Override Release flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "/Od" CACHE STRING "Override Release flags" FORCE)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O0" CACHE STRING "Override Release flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-O0" CACHE STRING "Override Release flags" FORCE)
endif()
 
include(cmake/installSDL3plus.cmake)

file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

sdl3plus_add_game(${PROJECT_NAME}
    SOURCES
        ${MAIN_SOURCES}
    
    ICON
        resources/icon.png
)

sdl3plus_add_android_key_file(${PROJECT_NAME} resources/release-keys.jks)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} PUBLIC GAME_VERSION="${CMAKE_PROJECT_VERSION}")


if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

b_embed(${PROJECT_NAME} resources/icon.png)
b_embed(${PROJECT_NAME} resources/atlas.png)
b_embed(${PROJECT_NAME} resources/Artifika_font.ttf)