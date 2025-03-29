#pragma once

#ifndef _PLAY_STATE_MANAGER_H_
#define _PLAY_STATE_MANAGER_H_

#include <string>
#include <vector>
#include "game_engine.h"
#include "SDL_mixer.h"
#include "../Menu/Button.h"
#include "../Menu/Menu.h"

class PlayStateManager
{
    private:

        std :: vector <std :: string> score_data;
        std :: vector <int> mute_channel;
        Engine* engine = nullptr;
        Menu_manager* pause_menu = nullptr;
        bool running = true;
        bool run_pause_menu = false;

    public:

        std :: vector <std :: string> pause_menu_button_text = {"Resume" , "New Game" , "How to Play" , "Sound: ON" , "Exit to Start Menu"};

        PlayStateManager();

        ~PlayStateManager();

        void handle_event(SDL_Event &e , SDL_Renderer* &renderer , bool &exit_to_main_menu , std :: vector <std :: string> &score_data);

        void new_game(SDL_Renderer* &renderer);

        void run_game(bool &exit_to_menu);

        void render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);
};

#endif // _PLAY_STATE_MANAGER_H_
