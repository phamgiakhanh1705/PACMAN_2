#pragma once

#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include "../Map/map.h"
#include "../Object/pacman.h"
#include "../Object/ghost.h"
#include "../Object/texture_source.h"
#include "../Manager/tick_manager.h"
#include "../Manager/game_manager.h"
#include "../Manager/Sound_manager.h"

class Engine
{
    public:

        static const int TILE_SIZE = 16;

        Engine()
        {
            map = nullptr;
            pacman = nullptr;
            blinky = nullptr;
            pinky = nullptr;
            clyde = nullptr;
            inky = nullptr;
            object_texture = nullptr;
            tick_manager = nullptr;
            game_manager = nullptr;
            sound_manager = nullptr;
        }

        ~Engine()
        {
            delete map;
            map = nullptr;

            delete pacman;
            pacman = nullptr;

            delete blinky;
            blinky = nullptr;

            delete pinky;
            pinky = nullptr;

            delete clyde;
            clyde = nullptr;

            delete inky;
            inky = nullptr;

            SDL_DestroyTexture(next_level);
            next_level = nullptr;

            SDL_DestroyTexture(ready);
            ready = nullptr;

            delete object_texture;
            object_texture = nullptr;

            delete tick_manager;
            tick_manager = nullptr;

            delete game_manager;
            game_manager = nullptr;

            delete sound_manager;
            sound_manager = nullptr;
        }

        void new_game();

        void init_game(SDL_Renderer* &renderer);

        void handle_event_engine(SDL_Event &event , std :: vector <std :: string> &score_data);

        void loop_game(bool &exit_to_menu);

        void render_game(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);

    private:

        Map* map;
        Pacman* pacman = nullptr;
        Ghost* blinky = nullptr;
        Ghost* pinky = nullptr;
        Ghost* inky = nullptr;
        Ghost* clyde = nullptr;
        Texture_Source* object_texture;
        TickManager* tick_manager;
        GameManager* game_manager;
        SoundManager* sound_manager;
        SDL_Texture* next_level;
        SDL_Texture* ready;
        bool running_endgame_board = false;
        int wait_time = 0;

    protected:

        void respawn_object();

        void ghost_move(Ghost* &ghost);

        void pacman_meat_ghost(Ghost* &ghost);

        void render_ghost(SDL_Renderer* &renderer , Ghost* &ghost , int ghost_id);
};

#endif // _GAME_ENGINE_H_
