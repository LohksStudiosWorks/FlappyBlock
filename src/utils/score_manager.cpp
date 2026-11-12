#include "utils/file_manager.hpp"
#include "utils/score_manager.hpp"

#include <SDL3/SDL.h>

#include <string>
#include <fstream>

ScoreManager::ScoreManager() {
    const std::string filepath = get_app_files_path("scores.txt");
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    SDL_Log("Loading score from: %s", filepath.c_str());
    
    if (!file.is_open()) 
        return;

    // Read the entire file content into a std::string
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    m_score = std::stoull(content);
}

ScoreManager::~ScoreManager() {
    if (!m_changed) return;

    const std::string filepath = get_app_files_path("scores.txt");
    SDL_Log("Attempt to store score at: %s", filepath.c_str());
    std::ofstream file(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
    
    if (file.is_open()) {
        file << m_score;
        SDL_Log("Stored score at: %s", filepath.c_str());
    } else {
        SDL_Log("Failed to open scores file for writing at: %s", filepath.c_str());
    }
}

uint64_t ScoreManager::get_score() {
    return m_score;
}

void ScoreManager::set_score_if_higher(uint64_t new_score) {
    if (m_score < new_score) {
        m_changed = true;
        m_score = new_score;
    }
}
