cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 2.0
    DESCRIPTION "FlappyBlock is a remake of a popular game made in C/C++ with SDL3"
    LANGUAGES C CXX
)

# Setup flags

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DUNICODE -D_UNICODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Include extras

if (MINGW)
    include(cmake/MinGWFix.cmake)
endif()

include(cmake/installCppHttpLib.cmake)
include(cmake/installBatteryEmbed.cmake)
include(cmake/installSDL3.cmake)

# Glob

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

# Core internal lib:

add_library(core STATIC ${CORE_SOURCES})

target_compile_features(core PUBLIC cxx_std_23)

set_target_properties(core PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(core PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    SDL3_image::SDL3_image
    #SDL3_net::SDL3_net
    httplib::httplib
)

if((UNIX AND NOT APPLE AND NOT ANDROID) OR MINGW)
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++"
    )
endif()

target_compile_definitions(core PUBLIC GAME_VERSION="${CMAKE_PROJECT_VERSION}")

# Main project:

if(ANDROID)
    set(TARGETING_NAME "main")
    target_compile_definitions(core PUBLIC IS_ANDROID=1)

    add_library(${TARGETING_NAME} SHARED 
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )
else()
    set(TARGETING_NAME "${PROJECT_NAME}")
    target_compile_definitions(core PUBLIC IS_ANDROID=0)

    add_executable(${TARGETING_NAME} WIN32 
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )
endif()

set_target_properties(${TARGETING_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/executable"
)

target_link_libraries(${TARGETING_NAME} PRIVATE core)

# Extra flags

if (MSVC)
    target_compile_options(core PRIVATE /W4)
    target_compile_options(${TARGETING_NAME} PRIVATE /W4)
else()
    target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(${TARGETING_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


# Embedding data

b_embed(core resources/icon.png)
b_embed(core resources/atlas.png)
b_embed(core resources/Artifika_font.ttf)
