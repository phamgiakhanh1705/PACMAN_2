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

class Menu_manager
{
    private:
        int alpha_mod = 1;
        int arrow_hs_pos;
        int current_button_id;
        int current_menu_status;
        int current_htp_id;

        int total_button_;
        int menu_button_width;
        int menu_button_height;
        int base_scr_pos_x;
        int base_scr_pos_y;

        Text_manager *score_text = nullptr;
        Mix_Chunk *navigation_sound = Mix_LoadWAV("Source/Assets/Sound/button.wav");
        Mix_Chunk *selection_sound = Mix_LoadWAV("Source/Assets/Sound/button.wav");
        SDL_Texture *menu_texture;
        SDL_Texture *how_to_play_texture[3];
        SDL_Texture *high_score_texture;
        SDL_Texture *arrow_texture;
        std :: vector <Menu_button*> menu_button;

        log_status *console = new log_status("Menu_manager");
        bool running = false;

        SDL_Texture* load_image(SDL_Renderer* &renderer , const std :: string img_path);

    public:
        static const int RESUME = 0;
        static const int RUNNING = 1;
        static const int PLAY_BUTTON_PRESSED = 2;
        static const int EXIT_BUTTON_PRESSED = 3;
        static const int HOW_TO_PLAY = 4;
        static const int HIGH_SCORES = 5;
        static const bool ON = true;
        static const bool OFF = false;

        Menu_manager(const int base_scr_pos_x , const int base_scr_pos_y , const int total_button , const int button_width , const int button_height);
        ~Menu_manager();

        void init(SDL_Renderer* &renderer , const std :: string img_path , std :: vector <std :: string> &button_text);
        void render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);
        void handle_event(SDL_Event &e , SDL_Renderer* &renderer);

        bool is_running() const;
        int get_status() const;
        void re_open();
        void change_run_status();
        bool get_sound_state() const;
};

#endif // _MENU_H_
