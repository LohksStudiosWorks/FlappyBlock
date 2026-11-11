#pragma once

#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>
#include <SDL3pp/SDL3pp_image.h>
#include <SDL3pp/SDL3pp_ttf.h>

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

    ResourceManager(SDL::Renderer& renderer);

    void draw_texture(const TextureId& id, const float x, const float y) const;
    void draw_texture_rotated(const TextureId& id, const float x, const float y, const double rot) const;
    void draw_text(const std::string_view& text, const float x, const float y, const SDL::Color color = {255,255,255,255});

    bool check_mouse_over(const TextureId& id, const SDL::FPointRaw draw_pos, const SDL::FPointRaw mouse_pos) const;
private:
    const SDL::FRect& get_texture_rect(const TextureId& id) const;

    const b::EmbedInternal::EmbeddedFile& m_atlas;
    const b::EmbedInternal::EmbeddedFile& m_icon_src;
    const b::EmbedInternal::EmbeddedFile& m_ttf_src;
    
    const SDL::Texture m_main;
    const SDL::Surface m_icon;
    const SDL::Font m_font;
    SDL::RendererTextEngine m_text_eng;
    SDL::Text m_text;

    SDL::Renderer& m_renderer_ref;

    std::unordered_map<TextureId, SDL::FRect> m_map;
};