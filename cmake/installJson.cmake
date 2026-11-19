# Setup dependency

include(FetchContent)

message(STATUS "Fetching JSON")

FetchContent_Declare(
    json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.12.0
)
FetchContent_MakeAvailable(json)

message(STATUS "Fetching of JSON done!")