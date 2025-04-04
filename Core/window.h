#pragma once

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include "play_state_manager.h"
#include "../Menu/button.h"

class Window
{
    public:
        static const int SCREEN_WIDTH = 882;
        static const int SCREEN_HEIGHT = 496;
        const std :: string WINDOW_TITLE = "Pacman";
        std :: vector <std :: string> start_menu_button_text = {"New Game" , "How to Play" , "High Scores" , "Sound: ON" , "Exit"};

        Window();
        ~Window();

        void init_SDL();

        void quit_SDL();

        void start_running_game();

    private:

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Menu* start_menu = nullptr;
        PlayStateManager* play_state = nullptr;
        log_status* Console = new log_status("Window");
        std :: vector <std :: string> highscores;
        bool running = false;
        bool menu_running = false;
};

#endif // _WINDOW_H_
