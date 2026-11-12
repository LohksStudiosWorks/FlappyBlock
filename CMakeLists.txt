cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 1.0
    DESCRIPTION "FlappyBlock is a copy of a popular game made in C/C++ with SDL3"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

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
)

# Avoid static lib flags on Android's Clang NDK toolchain
if((UNIX AND NOT APPLE AND NOT ANDROID) OR MINGW)
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++"
    )
endif()

target_compile_options(core PRIVATE
    $<$<CONFIG:Release>:-O3>
)


# ===== Platform-specific Target (Android Shared Library vs Desktop Executable) ===== #

if(ANDROID)
    # Android expects a shared library named "main" loaded via JNI by SDLActivity
    add_library(main SHARED 
        "${PROJECT_ROOT_DIR}/src/main.cpp"
    )

    target_link_libraries(main PRIVATE
        core
    )

    target_compile_options(main PRIVATE
        $<$<CONFIG:Release>:-O3>
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

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3>
    )
endif()


if(MSVC)
  target_compile_options(core PRIVATE /W4 /WX)
  if(ANDROID)
      target_compile_options(main PRIVATE /W4 /WX)
  else()
      target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  endif()
else()
  target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic -Werror)
  if(ANDROID)
      target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror)
  else()
      target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endif()
endif()

b_embed(core resources/icon.png)
b_embed(core resources/atlas.png)
b_embed(core resources/Artifika_font.ttf)

# ===== Enable testing (Desktop only) ===== #
if(NOT ANDROID)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()