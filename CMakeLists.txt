cmake_minimum_required(VERSION 3.28)

project(FlappyBlock
    VERSION 1.3.0
    DESCRIPTION "FlappyBlock is a remake of a popular game made in C/C++ with SDL3"
    LANGUAGES CXX
)
 
include(cmake/projectFlags.cmake)
include(cmake/installBatteryEmbed.cmake)
include(cmake/installSDL3.cmake)
include(cmake/installJson.cmake)
include(cmake/installMbedTLS.cmake)
include(cmake/installCppHttpLib.cmake)


# ===== Shared core code ===== #

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

list(REMOVE_ITEM CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

add_library(core STATIC ${CORE_SOURCES})

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


# ===== Platform-specific Target (Android Shared Library vs Desktop Executable) ===== #

if(ANDROID)
    # Android expects a shared library named "main" loaded via JNI by SDLActivity
    add_library(main SHARED 
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )

    target_link_libraries(main PRIVATE
        core
    )

    target_compile_definitions(core PUBLIC GAME_VERSION="${CMAKE_PROJECT_VERSION}" IS_ANDROID=1)
else()
    add_executable(${PROJECT_NAME} WIN32
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/executable"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        core
    )

    target_compile_definitions(core PUBLIC GAME_VERSION="${CMAKE_PROJECT_VERSION}" IS_ANDROID=0)
endif()

if(MSVC)
    target_compile_options(core PRIVATE /W4)
    if(ANDROID)
        target_compile_options(main PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    endif()
else()
    target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
    if(ANDROID)
        target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

b_embed(core resources/icon.png)
b_embed(core resources/atlas.png)
b_embed(core resources/Artifika_font.ttf)
b_embed(core resources/certificates/mozilla_certificate.pem)