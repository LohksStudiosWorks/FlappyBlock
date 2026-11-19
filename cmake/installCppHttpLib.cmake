# Setup dependency

include(FetchContent)

if(NOT TARGET mbedtls)
    include(cmake/installMbedTLS.cmake)
endif()

include(cmake/updateCertificates.cmake)

message(STATUS "Fetching CppHttpLib")

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

include(cmake/integrateCppHttpLib.cmake)

message(STATUS "Fetching of CppHttpLib done!")