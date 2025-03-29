#pragma once

#ifndef _MENU_MANAGER_H_
#define _MENU_MANAGER_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "../Menu/Button.h"
#include "../Manager/text_manager.h"

class Menu_manager
{
    private:

        std :: vector <Menu_button*> menu_buttons;
        std :: vector <Menu_button*> highscore_buttons;
        std :: vector <Menu_button*> instruction_buttons;

        std :: vector <std :: string> highscore_data;
        std :: vector <std :: string> instruction_data;

        SDL_Texture* background_texture;
        SDL_Texture* highscore_texture;
        SDL_Texture* instruction_texture;
        SDL_Texture* arrow_texture;

        Mix_Chunk* click_sound;

        Text_manager* highscore_title_text;

        int arrow_highscore_pos;
        int menu_state;
        int selected_menu_button;
        bool show_arrow;

    public:

        static const int STATE_MENU = 0;
        static const int STATE_HIGHSCORE = 1;
        static const int STATE_INSTRUCTION = 2;

        Menu_manager();

        ~Menu_manager();

        void initialize(SDL_Renderer* &renderer);

        void render(SDL_Renderer* &renderer);

        void handle_event(SDL_Event &event , bool &running , bool &play_game);

        int get_menu_state() const;

        std :: vector <std :: string> get_highscore_data() const;
};

#endif // _MENU_MANAGER_H_
