#pragma once

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "../Manager/text_manager.h"

class Button
{
    private:

        SDL_Rect button_rect;
        TextManager* normal_text;
        TextManager* select_text;
        TextManager* select_text_detail;

        int button_status;

        std :: string button_text;
        std :: string button_detail;

    public:

        static const int BUTTON_OUT = 0;
        static const int BUTTON_IN  = 1;
        static const int BUTTON_PRESSED = 2;
        static const int MENU_FONT_SIZE = 28;

        const SDL_Color normal_color = {0 , 0 , 0 , 255};
        const SDL_Color select_color = {254 , 233 , 0 , 255};

        Button(int width , int height , int screen_pos_col , int screen_pos_row);
        ~Button() {};

        void load_button(SDL_Renderer* &renderer , std :: string text = "");

        void render_button(SDL_Renderer* &renderer);

        void set_status(const int status);

        void change_sound_button(SDL_Renderer* &renderer);

        bool check_mouse_pos(const int &col , const int &row) const;

        std :: string get_text() const;
};

#endif // _BUTTON_H
