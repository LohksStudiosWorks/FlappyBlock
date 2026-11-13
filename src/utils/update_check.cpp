#include "utils/update_check.hpp"

#if (IS_ANDROID == 0)
#include <curl/curl.h>
#include <cstdint>
#include <regex>

constexpr char api_url[] = "https://gamejolt.com/site-api/web/discover/games/overview/1118179";
const std::regex api_regex("\"version_number\"\\s*:\\s*\"([\\d.]+)\"", std::regex_constants::icase);

static std::string download(const std::string& uri);


std::string get_latest_version() {
    const std::string data = download(api_url);
    std::smatch match;

    if (std::regex_search(data, match, api_regex) && match.size() > 1)
        return match[1].str();

    return "";
}


static size_t __WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    auto* buffer = static_cast<std::string*>(userp);    
    auto* data_ptr = static_cast<uint8_t*>(contents);
    buffer->insert(buffer->end(), data_ptr, data_ptr + total_size);    
    return total_size;
}

std::string download(const std::string& uri) {
    CURL* curl = curl_easy_init();
    if (!curl) { return {}; }

    std::string data;

    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "LSWE-Example/1.0");

    CURLcode res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);

    return res == CURLE_OK ? data : std::string{};
}
#endif