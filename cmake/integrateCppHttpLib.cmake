target_compile_definitions(httplib INTERFACE
    CPPHTTPLIB_MBEDTLS_SUPPORT
)

target_link_libraries(httplib INTERFACE    
    mbedtls
    mbedx509
    tfpsacrypto
)

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