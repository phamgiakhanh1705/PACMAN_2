#pragma once

#ifndef _PLAY_STATE_MANAGER_H_
#define _PLAY_STATE_MANAGER_H_

#include <string>
#include <vector>
#include "game_engine.h"
#include "SDL_mixer.h"
#include "../Menu/button.h"
#include "../Menu/menu.h"

class PlayStateManager
{
    public:

        std :: vector <std :: string> pause_menu_button_text = {"Resume" , "New Game" , "How to Play" , "Sound: ON" , "Exit to Start Menu"};

        PlayStateManager();
        ~PlayStateManager();

        void handle_event(SDL_Event &event , SDL_Renderer* &renderer , bool &runningMain , std :: vector <std :: string> &score_data);

        void new_game(SDL_Renderer* &renderer);

        void run_game(bool &exit_to_menu);

        void render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);

    private:

        std :: vector <std :: string> scoreData;
        std :: vector <int> mute_channel;
        Engine* engine = nullptr;
        Menu* pause_menu = nullptr;
        bool running = true;
        bool running_menu_pause = false;
};

#endif // _PLAY_STATE_MANAGER_H_
