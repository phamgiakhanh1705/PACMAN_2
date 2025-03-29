#pragma once

#ifndef _TEXTURE_SOURCE_H
#define _TEXTURE_SOURCE_H

#include <SDL.h>
#include <SDL_image.h>
#include "../LogStatus/logstatus.h"
#include "../Map/map.h"

class Texture_source
{
    private:

        log_status* logger;

        SDL_Texture* tile_texture;
        SDL_Rect tile_sprites[32];

        SDL_Texture* entity_texture;
        SDL_Texture* ghost_score_texture;

        SDL_Rect pacman_up[3];
        SDL_Rect pacman_down[3];
        SDL_Rect pacman_left[3];
        SDL_Rect pacman_right[3];
        SDL_Rect pacman_dead[11];

        SDL_Rect ghost_sprite[7][6][2];

        int pacman_frame;
        int ghost_frame[7];

    public:

        static const int up = 0;
        static const int right = 1;
        static const int down = 2;
        static const int left = 3;
        static const int frighten_ghost_1 = 4;
        static const int frighten_ghost_2 = 5;
        static const int dead_pacman = 5;

        static const int BLINKY = 0;
        static const int PINKY  = 1;
        static const int INKY   = 2;
        static const int CLYDE  = 3;
        static const int GREENDY = 4;
        static const int FRIENDY = 5;
        static const int ghost_spirit = 6;
        static const int total_ghost = 7;

        Texture_source();
        ~Texture_source();

        bool is_pacman_dead();
        void load_tile_texture(SDL_Renderer* &renderer);
        void render_tile_texture(SDL_Renderer* &renderer , int tile_id , SDL_Rect* dst_rect);
        void load_pacman_and_ghost_texture(SDL_Renderer* &renderer);
        void render_pacman(SDL_Renderer* &renderer , int pos_x , int pos_y , int status);
        void render_ghost(SDL_Renderer* &renderer , int pos_x , int pos_y , int ghost_id , int status);
        void render_ghost_score(SDL_Renderer* &renderer , int eaten_ghost_x , int eaten_ghost_y , int eaten_ghost_streak);
        void render_map(SDL_Renderer* &renderer , Map* map);
        void render_warning_effect(SDL_Renderer* &renderer);

};

#endif // _TEXTURE_SOURCE_H
