#pragma once

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "../Object/Ghost.h"
#include "text_manager.h"
#include "../Menu/Button.h"

class GameManager
{
    private:

        int level;
        int life;
        int eaten_coins;
        int eaten_ghosts;
        int scores;

        int pos = -1;
        std :: string player_name = "";
        int player_decision;
        int current_button;
        bool new_record = false;

        int ghost_is_eaten_in_pos_col;
        int ghost_is_eaten_in_pos_row;

        int PINKY_COIN_LIMIT;
        int INKY_COIN_LIMIT;
        int CLYDE_COIN_LIMIT;

        TextManager* level_text;
        TextManager* live_text;
        TextManager* score_text;
        TextManager* player_NAME;

        SDL_Texture* endgame_board;
        SDL_Texture* highscores_board;
        Button* button_yes;
        Button* button_no;
        Mix_Chunk* navigation_sound = Mix_LoadWAV("Assets/Sound/button.wav");

    protected:

        SDL_Texture* load_image(SDL_Renderer* &renderer , const std :: string image_path);

    public:

        const int TOTAL_COINS = 244;
        static const int NORMAL_COIN = 26;
        static const int SUPER_COIN = 27;
        static const int NOT_COIN = 0;
        static const int PAUSE_GAME = 1;
        static const int AGAIN = 2;
        static const int QUIT  = 3;
        static const int WAITING = 4;

        GameManager(SDL_Renderer* &renderer);
        ~GameManager();

        void reset_game();

        void next_level();

        void eat_coin(const int type_of_coin);

        void eat_ghost(const int ghost_tile_col , const int ghost_tile_row);

        void lost_a_life();

        int get_eaten_ghost_streak() const;

        int get_eaten_ghost_pos_col() const;

        int get_eaten_ghost_pos_row() const;

        int get_remain_life() const;

        bool eat_all_coins() const;

        int get_level() const;

        int get_player_decision() const;

        int get_remain_coins() const;

        void handle_ghost_pos(Ghost* &pinky , Ghost* &inky , Ghost* &clyde);

        void handle_endgame_board(SDL_Event &event , std :: vector <std :: string> &score_data);

        void render_HUD(SDL_Renderer* &renderer);

        void run_endgame_board(SDL_Renderer* &renderer);

        void check_score_data(const std :: vector <std :: string> &score_data);

};

#endif // GAMEMANAGER_H_
