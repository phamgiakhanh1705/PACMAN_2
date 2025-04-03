#include "text_manager.h"

TextManager :: TextManager(int font_size)
{
    font = TTF_OpenFont(FONT_NAME.c_str() , font_size);
    text_texture = nullptr;
    text_surface = nullptr;
}

TextManager :: ~TextManager()
{
    if(font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if(text_texture != nullptr) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    if(text_surface != nullptr) {
        SDL_FreeSurface(text_surface);
        text_surface = nullptr;
    }
}

int TextManager :: get_text_length() const
{
    return text_surface -> w;
}

void TextManager :: render_text(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , const int type)
{
    display_rect = {screen_pos_col , screen_pos_row , text_surface -> w , text_surface -> h};
    if(type == CENTER) {
        display_rect.x = screen_pos_col - text_surface -> w / 2;
        display_rect.y = screen_pos_row - text_surface -> h / 2;
    }
    SDL_RenderCopy(renderer , text_texture , nullptr , &display_rect);
}

void TextManager :: load_render_text(SDL_Renderer* &renderer , std :: string text , SDL_Color text_color)
{
    if(text_surface != nullptr) SDL_FreeSurface(text_surface);
    text_surface = TTF_RenderText_Blended(font , text.c_str() , text_color);

    if(text_texture != nullptr) SDL_DestroyTexture(text_texture);
    text_texture = SDL_CreateTextureFromSurface(renderer , text_surface);
}
