#pragma once

#include <memory>
#include <thread>

class DiscordManager {
public:
    DiscordManager();
    ~DiscordManager();

private:
    struct core;

    std::unique_ptr<core> m_discord;
    std::thread m_update;
    bool m_running = true;
};