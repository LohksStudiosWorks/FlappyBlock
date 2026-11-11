#include "utils/discord_sdk.hpp"

#define DISCORDPP_IMPLEMENTATION
#include "discordpp.h"

#include <battery/embed.hpp>

#include <iostream>
#include <fstream>

static void export_discord_lib();

struct DiscordManager::core{
    std::unique_ptr<discordpp::Client> m_client;
};

DiscordManager::DiscordManager() 
    : m_discord(std::make_unique<core>())
{
    export_discord_lib();

    const auto embed_id = b::embed<"resources/discord_app_id.txt">();
    const uint64_t embed_id_uint64 = std::stoull(embed_id.str());

    m_discord->m_client = std::make_unique<discordpp::Client>();
    m_discord->m_client->SetApplicationId(embed_id_uint64);

    discordpp::Activity activity;
    activity.SetType(discordpp::ActivityTypes::Playing);
    activity.SetState("Chasing high scores");
    activity.SetDetails("Playing FlappyBlock");

    m_discord->m_client->UpdateRichPresence(activity, [](discordpp::ClientResult result) {
        if (!result.Successful()) {
            std::cerr << "❌ Failed to update Rich Presence: " << result.ToString() << "\n";
        }
        std::cout << result.ToString() << std::endl;
    });

    m_update = std::thread([this]{
        while(m_running) {
            discordpp::RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}

DiscordManager::~DiscordManager() {
    m_running = false;
    m_update.join();
    std::remove(DISCORD_LIB_FILE);
}


void export_discord_lib(){
    const auto discord_lib = b::embed<DISCORD_LIB_NAME>();
    
    const std::string discord_lib_data = discord_lib.str();
    std::fstream out(DISCORD_LIB_FILE, std::ios::out | std::ios::binary);
    out.write(discord_lib_data.data(), discord_lib_data.size());
    out.flush();
    out.close();
}