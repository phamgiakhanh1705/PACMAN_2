#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include "../Map/Map.h"
#include "../Object/Pacman.h"
#include "../Object/Ghost.h"
#include "../Object/Item.h"
#include "../Object/texture_source.h"
#include "../Manager/tick_manager.h"
#include "../Manager/game_manager.h"
#include "../Manager/sound_manager.h"

class Engine
{
    private:

        Map* map;
        Pacman* pacman = nullptr;
        Ghost* blinky = nullptr;
        Ghost* pinky = nullptr;
        Ghost* inky = nullptr;
        Ghost* clyde = nullptr;
        Ghost* greendy = nullptr;
        Ghost* friendy = nullptr;
        Item* apple;
        Texture_source* object_texture;
        Tick_manager* tick_manager;
        Game_manager* game_manager;
        Sound_manager* sound_manager;
        SDL_Texture* next_level;
        SDL_Texture* ready;
        bool running_eg_board = false;
        bool eat_green_apple = false;
        int wait_time = 0;

    protected:

        void respawn_object();

        void ghost_move(Ghost* &ghost);

        void pacman_meat_ghost(Ghost* &ghost);

        void render_ghost(SDL_Renderer* &renderer , Ghost* &ghost , int ghost_id);

    public:

        Engine();

        ~Engine();

        void new_game();

        void init(SDL_Renderer* &renderer);

        void handle_event(SDL_Event &e , std :: vector <std :: string> &score_data);

        void loop(bool &exit_to_menu);

        void render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data);
};

#endif // _ENGINE_H_
