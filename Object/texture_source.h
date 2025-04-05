#pragma once

#ifndef _TEXTURE_SOURCE
#define _TEXTURE_SOURCE

#include <SDL.h>
#include <SDL_image.h>
#include "../LogStatus/logstatus.h"
#include <vector>

class Texture_Source
{
    public:

        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;

        static const int FRIGHTEN_GHOST_2 = 5;
        static const int FRIGHTEN_GHOST_1 = 4;
        static const int DEAD_PACMAN = 5;

        static const int BLINKY = 0;
        static const int PINKY = 1;
        static const int INKY = 2;
        static const int CLYDE = 3;
        static const int GREENDY = 4;
        static const int FRIENDY = 5;
        static const int GHOST_SPIRIT = 6;
        static const int TOTAL_GHOST = 6;

        static const int TILE_SIZE = 16;
        static const int ENTITY_SIZE = 30;

        Texture_Source();
        ~Texture_Source();

        bool is_pacman_dead();

        void load_tile_texture(SDL_Renderer* &renderer);

        void render_tile_texture(SDL_Renderer* &renderer , int tile_id , SDL_Rect* display_rect);

        void load_pacman_and_ghost_texture(SDL_Renderer* &renderer);

        void render_pacman_texture(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , int status);

        void render_ghost_texture(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , int ghost_id , int status);

        void render_ghost_score(SDL_Renderer* &renderer , const int eaten_ghost_pos_col , const int eaten_ghost_pos_row , const int eaten_ghost_streak);

    private:

        log_status* Console = new log_status("Texture_Source");

        SDL_Texture* tile_texture;
        SDL_Rect tile_sprite[32];

        SDL_Texture* entity_texture;
        SDL_Texture* ghost_score;

        SDL_Rect pacman_UP[3];
        SDL_Rect pacman_DOWN[3];
        SDL_Rect pacman_LEFT[3];
        SDL_Rect pacman_RIGHT[3];
        SDL_Rect pacman_DEAD[11];
        SDL_Rect ghost[7][6][2];

        int pacman_frame;
        int ghost_frame[7];

};

#endif // _TEXTURE_SOURCE
