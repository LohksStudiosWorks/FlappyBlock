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

static std::string download(const std::string& uri)
{
    CURL* curl = curl_easy_init();
    if (!curl)
        return {};

    std::string data;
    char error_buffer[CURL_ERROR_SIZE]{};

    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, __WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "FlappyBlock/2.0");

    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

    CURLcode res = curl_easy_perform(curl);

    long http_status = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "CURL ERROR [%d]: %s\n",
            res,
            error_buffer[0] ? error_buffer : curl_easy_strerror(res));
    }
    else
    {
        fprintf(stderr, "HTTP STATUS: %ld\n", http_status);
        fprintf(stderr, "RECEIVED: %zu bytes\n", data.size());

        if (http_status < 200 || http_status >= 300)
            fprintf(stderr, "RESPONSE: %.300s\n", data.c_str());
    }

    curl_easy_cleanup(curl);

    return (res == CURLE_OK && http_status >= 200 && http_status < 300)
        ? data
        : std::string{};
}

#endif