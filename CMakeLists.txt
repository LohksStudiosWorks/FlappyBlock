cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 2.0
    DESCRIPTION "FlappyBlock is a remake of a popular game made in C/C++ with SDL3"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Od" CACHE STRING "Override Release flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "/Od" CACHE STRING "Override Release flags" FORCE)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O0" CACHE STRING "Override Release flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "-O0" CACHE STRING "Override Release flags" FORCE)
endif()

set(PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DUNICODE -D_UNICODE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


if (EXISTS "${CMAKE_SOURCE_DIR}/.git" AND NOT ANDROID)
    execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()


if (MINGW)
    include(cmake/MinGWFix.cmake)
endif()

include(cmake/installLibCurl.cmake)
include(cmake/installBatteryEmbed.cmake)
include(cmake/installSDL3.cmake)


# ===== Shared core code ===== #

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_ROOT_DIR}/src/*.cpp
)

list(REMOVE_ITEM CORE_SOURCES
    "${PROJECT_ROOT_DIR}/src/main.cpp"
)

add_library(core STATIC ${CORE_SOURCES})

target_include_directories(core PUBLIC 
    ${PROJECT_ROOT_DIR}/include
)

target_link_libraries(core PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    SDL3_image::SDL3_image
    #SDL3_net::SDL3_net
    libcurl
)

if(NOT ANDROID)
    target_link_libraries(core PUBLIC
        ${CURL_LIBRARIES}
    )
endif()

# Avoid static lib flags on Android's Clang NDK toolchain
if((UNIX AND NOT APPLE AND NOT ANDROID) OR MINGW)
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++"
    )
endif()


# ===== Platform-specific Target (Android Shared Library vs Desktop Executable) ===== #

if(ANDROID)
    # Android expects a shared library named "main" loaded via JNI by SDLActivity
    add_library(main SHARED 
        "${PROJECT_ROOT_DIR}/src/main.cpp"
    )

    target_link_libraries(main PRIVATE
        core
    )
else()
    add_executable(${PROJECT_NAME} WIN32
        "${PROJECT_ROOT_DIR}/src/main.cpp"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/executable"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        core
    )
endif()



if(MSVC)
    target_compile_options(core PRIVATE /W4)
    if(ANDROID)
        target_compile_options(main PRIVATE /W4)
        add_compile_definitions(main PRIVATE GAME_VERSION="${CMAKE_PROJECT_VERSION}")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
        add_compile_definitions(${PROJECT_NAME} PRIVATE GAME_VERSION="${CMAKE_PROJECT_VERSION}")
    endif()
else()
    target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
    if(ANDROID)
        target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
        add_compile_definitions(main PRIVATE GAME_VERSION="${CMAKE_PROJECT_VERSION}")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
        add_compile_definitions(${PROJECT_NAME} PRIVATE GAME_VERSION="${CMAKE_PROJECT_VERSION}")
    endif()
endif()

b_embed(core resources/icon.png)
b_embed(core resources/atlas.png)
b_embed(core resources/Artifika_font.ttf)