cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 1.3.1
    DESCRIPTION "FlappyBlock is a remake of a popular game made in C/C++ with SDL3_plus"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DUNICODE -D_UNICODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(BUILD_SHARED_LIBS OFF)

if (MINGW)
    include(cmake/fixMinGW.cmake)
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
    STUDIO
        lohksstudiosworks
)

sdl3plus_add_android_key_file(${PROJECT_NAME} resources/release-key.jks)


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