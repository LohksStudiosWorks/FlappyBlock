include(FetchContent)

set(BUILD_CURL_EXE OFF CACHE BOOL "Disable curl executable" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable curl tests" FORCE)

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG        curl-8_23_0
)

FetchContent_MakeAvailable(curl)