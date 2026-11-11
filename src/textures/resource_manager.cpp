#include "textures/resource_manager.hpp"

constexpr float c_font_size = 10.0f;

SDL::Surface load_surface_from_mem(SDL::SourceBytes bytes);
SDL::Font load_font_from_mem(SDL::SourceBytes bytes, float ptsize);
SDL::Texture load_texture_from_mem(SDL::Renderer& renderer, SDL::SourceBytes bytes);

ResourceManager::ResourceManager(SDL::Renderer& renderer) :
    m_atlas(b::embed<"resources/atlas.png">()),
    m_icon_src(b::embed<"resources/icon.png">()),
    m_ttf_src(b::embed<"resources/Artifika_font.ttf">()),
    m_main(load_texture_from_mem(renderer, {m_atlas.data(), m_atlas.size()})),
    m_icon(load_surface_from_mem({m_icon_src.data(), m_icon_src.length()})),
    m_font(load_font_from_mem({m_ttf_src.data(), m_ttf_src.size()}, c_font_size)),
    m_text_eng(SDL::CreateRendererTextEngine(renderer)),
    m_text(m_text_eng.CreateText(m_font, "Test")),
    m_renderer_ref(renderer)
{
    m_map[TextureId::BG_LIGHT]          = {0.0f, 0.0f, 144.0f, 256.0f};
    m_map[TextureId::BG_DARK]           = {146.0f, 0.0f, 144.0f, 256.0f};
    m_map[TextureId::FLOOR]             = {292.0f, 0.0f, 168.0f, 56.0f};
    m_map[TextureId::PIPE_GREEN_UP]     = {84.0f, 323.0f, 26.0f, 160.0f};
    m_map[TextureId::PIPE_GREEN_DOWN]   = {56.0f, 323.0f, 26.0f, 160.0f};
    m_map[TextureId::PIPE_COPPER_UP]    = {28.0f, 323.0f, 26.0f, 160.0f};
    m_map[TextureId::PIPE_COPPER_DOWN]  = { 0.0f, 323.0f, 26.0f, 160.0f};
    m_map[TextureId::SIGN_GET_READY]    = {293.0f, 59.0f, 96.0f, 26.0f};
    m_map[TextureId::SIGN_GAME_OVER]    = {395.0f, 59.0f, 96.0f, 26.0f};
    m_map[TextureId::SIGN_TAP_HINT]     = {292.0f, 91.0f, 57.0f, 49.0f};
    m_map[TextureId::MENU_LOGO]         = {351.0f, 91.0f, 89.0f, 24.0f};
    m_map[TextureId::BUTTON_START]      = {354.0f, 118.0f, 52.0f, 29.0f};
    m_map[TextureId::BUTTON_SCORES]     = {414.0f, 118.0f, 52.0f, 29.0f};
    m_map[TextureId::BIRD_0_FRAME_0]    = {1.0f, 487.0f, 20.0f, 20.0f};
    m_map[TextureId::BIRD_0_FRAME_1]    = {29.0f, 487.0f, 20.0f, 20.0f};
    m_map[TextureId::BIRD_0_FRAME_2]    = {57.0f, 487.0f, 20.0f, 20.0f};
}

void ResourceManager::draw_texture(const TextureId& id, const float x, const float y) const {
    const auto& rec = get_texture_rect(id);
    m_renderer_ref.RenderTexture(m_main, rec, SDL::FRectRaw{x, y, rec.w, rec.h});
}

void ResourceManager::draw_texture_rotated(const TextureId& id, const float x, const float y, const double rot) const {
    const auto& rec = get_texture_rect(id);
    m_renderer_ref.RenderTextureRotated(m_main, rec, SDL::FRectRaw{x, y, rec.w, rec.h}, rot, std::nullopt);
}

void ResourceManager::draw_text(const std::string_view& text, const float x, const float y, const SDL::Color color) {
    m_text.SetString(text);
    m_text.SetColor(color);
    m_text.DrawRenderer({x, y});
}

bool ResourceManager::check_mouse_over(const TextureId& id, const SDL::FPointRaw draw_pos, const SDL::FPointRaw mouse_pos) const {
    SDL::FRect rec = get_texture_rect(id);
    rec.x = draw_pos.x;
    rec.y = draw_pos.y;    
    return SDL::PointInRectFloat(mouse_pos, rec);
}

const SDL::FRect& ResourceManager::get_texture_rect(const TextureId& id) const {
    const auto it = m_map.find(id);
    if (it != m_map.end()) return it->second;
    SDL::Log("ERROR: Requested invalid texture.");
    throw 1;
}

SDL::Surface load_surface_from_mem(SDL::SourceBytes bytes) {
    SDL::IOStream io = SDL::IOFromConstMem(bytes);
    return SDL::LoadSurface_IO(io.release(), true); // true = auto-close IO stream
}

SDL::Font load_font_from_mem(SDL::SourceBytes bytes, float ptsize) {
    SDL::IOStream io = SDL::IOFromConstMem(bytes);
    return SDL::OpenFontIO(io.release(), ptsize, true); // true = auto-close IO stream
}

SDL::Texture load_texture_from_mem(SDL::Renderer& renderer, SDL::SourceBytes bytes) {
    SDL::IOStream io = SDL::IOFromConstMem(bytes);
    SDL::Surface surface(io.release(), true);
    SDL::Texture texture(renderer, surface);
    texture.SetScaleMode(SDL::SCALEMODE_NEAREST);
    return texture;
}