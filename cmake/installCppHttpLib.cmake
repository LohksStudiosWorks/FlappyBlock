
include(FetchContent)

# --------------------------------------------------
# Mbed TLS
# --------------------------------------------------

set(USE_SHARED_MBEDTLS_LIBRARY OFF CACHE BOOL "" FORCE)
set(USE_STATIC_MBEDTLS_LIBRARY ON CACHE BOOL "" FORCE)
set(ENABLE_PROGRAMS OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    mbedtls
    GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
    GIT_TAG v4.2.0
    GIT_SHALLOW TRUE
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_MakeAvailable(mbedtls)

foreach(target mbedtls mbedx509 tfpsacrypto)
    if(NOT TARGET ${target})
        message(FATAL_ERROR
            "Missing Mbed target: ${target}"
        )
    endif()
endforeach()

# --------------------------------------------------
# cpp-httplib
# --------------------------------------------------

set(HTTPLIB_COMPILE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_INSTALL OFF CACHE BOOL "" FORCE)
set(HTTPLIB_TEST OFF CACHE BOOL "" FORCE)

set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_MBEDTLS_IF_AVAILABLE OFF CACHE BOOL "" FORCE)

set(HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.66.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(httplib)

# Explicitly configure the TLS backend.
target_compile_definitions(httplib INTERFACE
    CPPHTTPLIB_MBEDTLS_SUPPORT
)

target_link_libraries(httplib INTERFACE    
    mbedtls
    mbedx509
    tfpsacrypto
)

# --------------------------------------------------
# Platform libraries
# --------------------------------------------------

if(WIN32)
    target_link_libraries(httplib INTERFACE
        ws2_32
        crypt32
    )
endif()

if(APPLE)
    target_link_libraries(httplib INTERFACE
        "-framework Security"
        "-framework CoreFoundation"
    )
endif()

if(ANDROID)
    target_link_libraries(httplib INTERFACE
        android
        log
    )
endif()