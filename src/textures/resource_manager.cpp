#include "textures/resource_manager.hpp"

constexpr float c_font_size = 10.0f;

SDL_Surface* load_surface_from_mem(const char* data, const size_t len);
TTF_Font* load_font_from_mem(const char* data, const size_t len, float ptsize);
SDL_Texture* load_texture_from_mem(SDL_Renderer* renderer, const char* data, const size_t len);

ResourceManager::ResourceManager(SDL_Renderer* renderer) :
    m_atlas(b::embed<"resources/atlas.png">()),
    m_icon_src(b::embed<"resources/icon.png">()),
    m_ttf_src(b::embed<"resources/Artifika_font.ttf">()),
    m_main(load_texture_from_mem(renderer, m_atlas.data(), m_atlas.size())),
    m_icon(load_surface_from_mem(m_icon_src.data(), m_icon_src.length())),
    m_font(load_font_from_mem(m_ttf_src.data(), m_ttf_src.size(), c_font_size)),
    m_text_eng(TTF_CreateRendererTextEngine(renderer)),
    m_text(TTF_CreateText(m_text_eng, m_font, "Test", 0)),
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

ResourceManager::~ResourceManager() {
    TTF_DestroyText(m_text);
    TTF_DestroyRendererTextEngine(m_text_eng);
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_main);
    SDL_DestroySurface(m_icon);
}

SDL_Surface* ResourceManager::get_icon() const {
    return m_icon;
}

void ResourceManager::draw_texture(const TextureId& id, const float x, const float y) const {
    const auto& rec = get_texture_rect(id);
    SDL_FRect dstrect{x, y, rec.w, rec.h};
    SDL_RenderTexture(m_renderer_ref, m_main, &rec, &dstrect);
}

void ResourceManager::draw_texture_rotated(const TextureId& id, const float x, const float y, const double rot) const {
    const auto& rec = get_texture_rect(id);
    SDL_FRect dstrect{x, y, rec.w, rec.h};
    SDL_FPoint center{rec.w / 2.0f, rec.h / 2.0f};
    SDL_RenderTextureRotated(m_renderer_ref, m_main, &rec, &dstrect, rot, &center, SDL_FLIP_NONE);
}

void ResourceManager::draw_text(const std::string_view& text, const float x, const float y, const SDL_Color color) {
    TTF_SetTextString(m_text, text.data(), text.size());
    TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
    TTF_DrawRendererText(m_text, x, y);
}

bool ResourceManager::check_mouse_over(const TextureId& id, const SDL_FPoint draw_pos, const SDL_FPoint mouse_pos) const {
    SDL_FRect rec = get_texture_rect(id);
    rec.x = draw_pos.x;
    rec.y = draw_pos.y;    
    return SDL_PointInRectFloat(&mouse_pos, &rec);
}

const SDL_FRect& ResourceManager::get_texture_rect(const TextureId& id) const {
    const auto it = m_map.find(id);
    if (it != m_map.end()) return it->second;
    SDL_Log("ERROR: Requested invalid texture.");
    throw 1;
}

SDL_Surface* load_surface_from_mem(const char* data, const size_t len) {
    SDL_IOStream* io = SDL_IOFromConstMem(data, len);
    return SDL_LoadSurface_IO(io, true); // true = auto-close IO stream
}

TTF_Font* load_font_from_mem(const char* data, const size_t len, float ptsize) {
    SDL_IOStream* io = SDL_IOFromConstMem(data, len);
    return TTF_OpenFontIO(io, true, ptsize); // true = auto-close IO stream
}

SDL_Texture* load_texture_from_mem(SDL_Renderer* renderer, const char* data, const size_t len) {
    SDL_IOStream* io = SDL_IOFromConstMem(data, len);
    SDL_Surface* surface = IMG_Load_IO(io, true);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(surface);
    return texture;
}