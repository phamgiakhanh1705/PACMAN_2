#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "../LogStatus/logstatus.h"
#include "Button.h"

class Menu
{
    private:

        int alpha_mod = 1;
        int arrow_HS_pos;
        int current_button_id;
        int current_menu_status;
        int current_HTP_id;

        int TOTAL_BUTTON;
        int MENU_BUTTON_WIDTH;
        int MENU_BUTTON_HEIGHT;
        int base_screen_pos_col;
        int base_screen_pos_row;

        TextManager* score_text = nullptr;
        Mix_Chunk* navigation_sound = Mix_LoadWAV("Assets/Sound/button.wav");
        Mix_Chunk* selection_sound = Mix_LoadWAV("Assets/Sound/button.wav");
        SDL_Texture* menu_texture;
        SDL_Texture* how_to_play_texture[3];
        SDL_Texture* high_score_texture;
        SDL_Texture* arrow_texture;
        std :: vector <Button*> menu_button;

        log_status* console = new log_status("Menu");
        bool running = false;

        SDL_Texture* load_image(SDL_Renderer* &renderer , const std :: string image_path);

    public:

        static const int RESUME = 0;
        static const int RUNNING = 1;
        static const int PLAY_BUTTON_PRESSED = 2;
        static const int EXIT_BUTTON_PRESSED = 3;
        static const int HOW_TO_PLAY = 4;
        static const int HIGH_SCORES = 5;
        static const bool ON = true;
        static const bool OFF = false;

        Menu(const int _base_screen_pos_col , const int _base_screen_pos_row , const int total_button , const int button_width , const int button_height);
        ~Menu();

        int get_menu_status() const;

        void init(SDL_Renderer* &renderer , const std :: string image_path , std :: vector <std :: string> &button_text);

        void render_menu(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);

        void handle_event(SDL_Event &event , SDL_Renderer* &renderer);

        void return_main_menu();

        void change_running_status();

        bool is_menu_running() const;

        bool get_sound_status() const;

};

#endif // _MENU_H_
