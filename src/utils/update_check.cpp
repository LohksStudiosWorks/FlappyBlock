#include "utils/update_check.hpp"

#include <httplib.h>
#include <cstdint>
#include <sstream>
#include <regex>
#include <iostream>
#include <battery/embed.hpp>

constexpr char api_url[] = "https://gamejolt.com";
constexpr char api_path[] = "/site-api/web/discover/games/overview/1118179";
const std::regex api_regex("\"version_number\"\\s*:\\s*\"([\\d.]+)\"", std::regex_constants::icase);

static std::string download();

std::string get_latest_version() {
    std::string data = download();
    if (data.empty() || data[0] == '#') return data;

    std::smatch match;

    if (std::regex_search(data, match, api_regex) && match.size() > 1)
        return match[1].str();

    return "";
}

std::string download() {
    httplib::Client cli(api_url);

    const auto embed = b::embed<"resources/certificates/mozilla_certificate.pem">();
    cli.load_ca_cert_store(embed.data(), embed.size());
    
    auto res = cli.Get(api_path);

    if (!res) {
        const auto err = res.error();
        std::stringstream err_str;

        switch (err) {
            case httplib::Error::SSLConnection:{
                char error_buffer[256]{};

                const auto backend_error =
                    static_cast<int>(res.ssl_backend_error());

                mbedtls_strerror(
                    backend_error,
                    error_buffer,
                    sizeof(error_buffer)
                );

                err_str << "SSL connection failed\n"
                        << "TLS error: " << res.ssl_error() << '\n'
                        << "MbedTLS error: " << backend_error << '\n'
                        << "Description: " << error_buffer;
            }
                break;
            case httplib::Error::SSLLoadingCerts:
                err_str << "SSL cert loading failed, backend error: "
                        << std::hex << res.ssl_backend_error();
            break;

            case httplib::Error::SSLServerVerification:
                err_str << "SSL verification failed, verify error: "
                        << res.ssl_backend_error();
            break;

            case httplib::Error::SSLServerHostnameVerification:
                err_str << "SSL hostname verification failed, verify error: "
                        << res.ssl_backend_error();
            break;

            default:
                err_str << "HTTP error: " << httplib::to_string(err);
                break;
        }

        return "#" + err_str.str();
    }

    return res->body;
}