#pragma once

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "../Manager/text_manager.h"

class Menu_button
{
    private:

        SDL_Rect button_rect;

        Text_manager* default_label; //
        Text_manager* highlighted_label;
        Text_manager* detail_label;

        int state;

        std :: string label_text;
        std :: string detail_text;

    public:

        //
        static const int STATE_NORMAL = 0;
        static const int STATE_HOVERED = 1;
        static const int STATE_PRESSED = 2;

        static const int MAIN_LABEL_FONT_SIZE = 24;
        static const int DETAIL_LABEL_FONT_SIZE = 18;

        static const int DEFAULT_FONT_SIZE = 28;

        const SDL_Color default_color = {0 , 0 , 0 , 255};
        const SDL_Color select_color = {254 , 233 , 0 , 255};

        //
        Menu_button(int width , int height , int x , int y);

        //
        void load_label(SDL_Renderer* &renderer , std :: string text = "");

        //
        void button_render(SDL_Renderer* &renderer);

        //
        void set_state(const int new_state);

        //
        void change_sound_button(SDL_Renderer* &renderer);

        //
        bool is_mouse_over(const int &x , const int &y) const;

        //
        std :: string get_label() const;

};
#endif // _BUTTON_H_
