include(FetchContent)

set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries" FORCE)

# Disable unnecessary curl targets
set(BUILD_CURL_EXE OFF CACHE BOOL "Disable curl executable" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable curl examples" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable curl tests" FORCE)
set(BUILD_LIBCURL_DOCS OFF CACHE BOOL "Disable curl documentation" FORCE)
set(BUILD_MISC_DOCS OFF CACHE BOOL "Disable curl miscellaneous documentation" FORCE)

# Optional dependencies
set(CURL_USE_LIBPSL OFF CACHE BOOL "Disable libpsl" FORCE)
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "Disable libssh2" FORCE)

# Compression and HTTP/2
set(CURL_BROTLI ON CACHE STRING "Enable Brotli" FORCE)
set(CURL_ZLIB ON CACHE STRING "Enable zlib" FORCE)
set(CURL_ZSTD ON CACHE STRING "Enable Zstandard" FORCE)

set(USE_NGHTTP2 ON CACHE BOOL "Enable HTTP/2" FORCE)
set(USE_LIBIDN2 ON CACHE BOOL "Enable IDN2" FORCE)

# Windows-specific configuration
if(WIN32)
    # Native Windows TLS implementation
    set(CURL_USE_SCHANNEL ON CACHE BOOL "Use Windows Schannel" FORCE)
    set(CURL_USE_OPENSSL OFF CACHE BOOL "Disable OpenSSL" FORCE)

    # Match the x64-windows-static vcpkg triplet
    if(MSVC)
        set(CURL_STATIC_CRT ON CACHE BOOL "Use static MSVC CRT" FORCE)
    endif()
endif()

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG        curl-8_23_0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(curl)