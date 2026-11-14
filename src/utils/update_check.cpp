#include "utils/update_check.hpp"

#include <httplib.h>
#include <cstdint>
#include <regex>
#include <iostream>

constexpr char api_url[] = "https://gamejolt.com";
constexpr char api_path[] = "/site-api/web/discover/games/overview/1118179";
const std::regex api_regex("\"version_number\"\\s*:\\s*\"([\\d.]+)\"", std::regex_constants::icase);

static std::string download();

std::string get_latest_version() {
    const std::string data = download();
    std::smatch match;

    if (std::regex_search(data, match, api_regex) && match.size() > 1)
        return match[1].str();

    return "";
}

std::string download() {
    httplib::Client cli(api_url);
    auto res = cli.Get(api_path);

    if (!res) {
        const auto err = res.error();

        switch (err) {
            case httplib::Error::SSLConnection:
                std::cout << "SSL connection failed, SSL error: "
                        << res.ssl_error() << std::endl;
            break;

            case httplib::Error::SSLLoadingCerts:
                std::cout << "SSL cert loading failed, backend error: "
                        << std::hex << res.ssl_backend_error() << std::endl;
            break;

            case httplib::Error::SSLServerVerification:
                std::cout << "SSL verification failed, verify error: "
                        << res.ssl_backend_error() << std::endl;
            break;

            case httplib::Error::SSLServerHostnameVerification:
                std::cout << "SSL hostname verification failed, verify error: "
                        << res.ssl_backend_error() << std::endl;
            break;

            default:
                std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
                break;
        }
        return "";
    }

    return res->body;
}