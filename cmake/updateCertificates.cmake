if(NOT DEFINED PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "PROJECT_SOURCE_DIR was not provided.")
endif()

set(CERTIFICATE_URL
    "https://curl.se/ca/cacert.pem"
)

set(CERTIFICATE_DIR
    "${PROJECT_SOURCE_DIR}/resources/certificates"
)

set(CERTIFICATE_FILE
    "${CERTIFICATE_DIR}/mozilla_certificate.pem"
)

set(TEMP_FILE
    "${CERTIFICATE_FILE}.tmp"
)

if(EXISTS "${CERTIFICATE_FILE}")
    message(STATUS "Skipping download of Mozilla CA certificates (file already exists).")
    return()
endif()


file(MAKE_DIRECTORY "${CERTIFICATE_DIR}")

message(STATUS "Checking for updated Mozilla CA certificates...")

file(DOWNLOAD
    "${CERTIFICATE_URL}"
    "${TEMP_FILE}"
    TLS_VERIFY ON
    TIMEOUT 60
    INACTIVITY_TIMEOUT 30
    STATUS DOWNLOAD_STATUS
    SHOW_PROGRESS
)

list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
list(GET DOWNLOAD_STATUS 1 DOWNLOAD_MESSAGE)

if(NOT DOWNLOAD_CODE EQUAL 0 )
    file(REMOVE "${TEMP_FILE}")

    if(EXISTS "${CERTIFICATE_FILE}")
        message(WARNING
            "Could not update Mozilla CA certificates.\n"
            "Reason: ${DOWNLOAD_MESSAGE}\n"
            "Keeping the previous certificate bundle."
        )
        return()
    endif()

    message(FATAL_ERROR
        "Failed to download Mozilla CA certificates.\n"
        "Reason: ${DOWNLOAD_MESSAGE}"
    )
endif()

# Validate that the downloaded file contains PEM certificates.
file(STRINGS
    "${TEMP_FILE}"
    CERTIFICATE_MARKER
    REGEX "-----BEGIN CERTIFICATE-----"
    LIMIT_COUNT 1
)

if(NOT CERTIFICATE_MARKER)
    file(REMOVE "${TEMP_FILE}")

    message(FATAL_ERROR
        "Downloaded Mozilla CA bundle is invalid."
    )
endif()

# Replace the previous bundle only after successful validation.
file(RENAME
    "${TEMP_FILE}"
    "${CERTIFICATE_FILE}"
)

message(STATUS
    "Mozilla CA certificates successfully updated:\n"
    "  ${CERTIFICATE_FILE}"
)
