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
        std :: vector<std :: string> pauseMenuButtonText = {"Resume" , "New Game" , "How to Play" , "Sound: ON" , "Exit to Start Menu"};

        PlayStateManager();

        ~PlayStateManager();

        void handleEvent(SDL_Event &e , SDL_Renderer* &renderer , bool &runningMain , std :: vector<std :: string> &scoreData);

        void newGame(SDL_Renderer* &renderer);

        void runGame(bool &exitToMenu);

        void render(SDL_Renderer* &renderer , const std :: vector<std :: string> &scoreData);

    private:
        std :: vector<std :: string> scoreData;
        std :: vector<int> muteChannel;
        Engine* engine = nullptr;
        Menu* pauseMenu = nullptr;
        bool running = true;
        bool runPauseMenu = false;
};

#endif // _PLAY_STATE_MANAGER_H_
