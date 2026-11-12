#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <battery/embed.hpp>

#include <unordered_map>

class ResourceManager {
public:
    enum class TextureId {
        BG_LIGHT,
        BG_DARK,
        FLOOR,
        PIPE_GREEN_UP,
        PIPE_GREEN_DOWN,
        PIPE_COPPER_UP,
        PIPE_COPPER_DOWN,
        SIGN_GET_READY,
        SIGN_GAME_OVER,
        SIGN_TAP_HINT,
        MENU_LOGO,
        BUTTON_START,
        BUTTON_SCORES,
        BIRD_0_FRAME_0,
        BIRD_0_FRAME_1,
        BIRD_0_FRAME_2,
    };

    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    SDL_Surface* get_icon() const;

    void draw_texture(const TextureId& id, const float x, const float y) const;
    void draw_texture_rotated(const TextureId& id, const float x, const float y, const double rot) const;
    void draw_text(const std::string_view& text, const float x, const float y, const SDL_Color color = {255,255,255,255});

    bool check_mouse_over(const TextureId& id, const SDL_FPoint draw_pos, const SDL_FPoint mouse_pos) const;
    bool check_texture_over_texture(const TextureId& id_0, const SDL_FPoint draw_pos_0, const TextureId& id_1, const SDL_FPoint draw_pos_1);
private:
    const SDL_FRect& get_texture_rect(const TextureId& id) const;

    const b::EmbedInternal::EmbeddedFile m_atlas;
    const b::EmbedInternal::EmbeddedFile m_icon_src;
    const b::EmbedInternal::EmbeddedFile m_ttf_src;
    
    SDL_Texture* m_main;
    SDL_Surface* m_icon;
    TTF_Font* m_font;
    TTF_TextEngine* m_text_eng;
    TTF_Text* m_text;

    SDL_Renderer* m_renderer_ref;

    std::unordered_map<TextureId, SDL_FRect> m_map;
};