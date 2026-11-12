#include "textures/resource_manager.hpp"

constexpr float c_font_size = 10.0f;

static SDL_Surface* load_surface_from_mem(const char* data, const size_t len);
static TTF_Font* load_font_from_mem(const char* data, const size_t len, float ptsize);
static SDL_Texture* load_texture_from_mem(SDL_Renderer* renderer, const char* data, const size_t len);
static std::string get_safe(const std::unordered_map<std::string, std::string>& cfg, const std::string& key);

ResourceManager::ResourceManager(SDL_Renderer* renderer, const std::unordered_map<std::string, std::string>& cfg) :
    m_atlas(b::embed<"resources/atlas.png">()),
    m_icon_src(b::embed<"resources/icon.png">()),
    m_ttf_src(b::embed<"resources/Artifika_font.ttf">()),
    m_main(load_texture_from_mem(renderer, m_atlas.data(), m_atlas.size())),
    m_icon(load_surface_from_mem(m_icon_src.data(), m_icon_src.length())),
    m_font(get_safe(cfg, "debugtext") != "1" ? nullptr : load_font_from_mem(m_ttf_src.data(), m_ttf_src.size(), c_font_size)),
    m_text_eng(get_safe(cfg, "debugtext") != "1" ? nullptr : TTF_CreateRendererTextEngine(renderer)),
    m_text(get_safe(cfg, "debugtext") != "1" ? nullptr : TTF_CreateText(m_text_eng, m_font, "Test", 0)),
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
    m_map[TextureId::END_FRAME_SCORE]   = {3.0f, 259.0f, 113.0f, 57.0f};
    m_map[TextureId::MEDAL_BRONZE]      = {112.0f, 477.0f, 22.0f, 22.0f};
    m_map[TextureId::MEDAL_SILVER]      = {112.0f, 453.0f, 22.0f, 22.0f};
    m_map[TextureId::MEDAL_GOLD]        = {121.0f, 282.0f, 22.0f, 22.0f};
    m_map[TextureId::MEDAL_PLATINUM]    = {121.0f, 258.0f, 22.0f, 22.0f};
    m_map[TextureId::NUMBER_0]          = {496.0f,  60.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_1]          = {134.0f, 455.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_2]          = {292.0f, 160.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_3]          = {306.0f, 160.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_4]          = {320.0f, 160.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_5]          = {334.0f, 160.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_6]          = {292.0f, 184.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_7]          = {306.0f, 184.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_8]          = {320.0f, 184.0f, 12.0f, 18.0f};
    m_map[TextureId::NUMBER_9]          = {334.0f, 184.0f, 12.0f, 18.0f};
    m_map[TextureId::SMALL_NUMBER_0]    = {138.0f, 323.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_1]    = {138.0f, 332.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_2]    = {138.0f, 349.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_3]    = {138.0f, 358.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_4]    = {138.0f, 375.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_5]    = {138.0f, 384.0f, 6.0f, 7.0f}; 
    m_map[TextureId::SMALL_NUMBER_6]    = {138.0f, 401.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_7]    = {138.0f, 410.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_8]    = {138.0f, 427.0f, 6.0f, 7.0f};
    m_map[TextureId::SMALL_NUMBER_9]    = {138.0f, 436.0f, 6.0f, 7.0f};
}

ResourceManager::~ResourceManager() {
    if (m_text) TTF_DestroyText(m_text);
    if (m_text_eng) TTF_DestroyRendererTextEngine(m_text_eng);
    if (m_font) TTF_CloseFont(m_font);
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
    if (!m_text) return;
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

bool ResourceManager::check_texture_over_texture(const TextureId& id_0, const SDL_FPoint draw_pos_0, const TextureId& id_1, const SDL_FPoint draw_pos_1) {
    SDL_FRect rec_0 = get_texture_rect(id_0);
    SDL_FRect rec_1 = get_texture_rect(id_1);
    rec_0.x = draw_pos_0.x;
    rec_0.y = draw_pos_0.y;
    rec_1.x = draw_pos_1.x;
    rec_1.y = draw_pos_1.y;
    return SDL_HasRectIntersectionFloat(&rec_0, &rec_1);

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

std::string get_safe(const std::unordered_map<std::string, std::string>& cfg, const std::string& key) {
    const auto it = cfg.find(key);
    if (it != cfg.end()) return it->second;
    return "";
}