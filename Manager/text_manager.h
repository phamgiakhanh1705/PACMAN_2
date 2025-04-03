#pragma once

#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include <SDL_ttf.h>
#include <string>

class TextManager
{
    private:

        SDL_Rect display_rect;
        TTF_Font* font;
        SDL_Texture* text_texture;
        SDL_Surface* text_surface;

    public:

        const std :: string FONT_NAME = "Assets/Font/Blogger Sans-Bold.ttf";
        static const int CENTER = 0;
        static const int LEFT = 1;

        TextManager(int font_size);
        ~TextManager();

        int get_text_length() const;

        void load_render_text(SDL_Renderer* &renderer , std :: string text , SDL_Color text_color);

        void render_text(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , const int type);

};

#endif // _TEXTMANAGER_H_
