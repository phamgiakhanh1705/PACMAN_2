#pragma once

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <SDL.h>
#include <string>
#include <vector>
#include "../Manager/text_manager.h"
#include "../Menu/button.h"
#include <SDL_mixer.h>

class Game_manager
{
    public:

        static const int STATE_WAITING = 0;
        static const int STATE_AGAIN   = 1;
        static const int STATE_QUIT    = 2;

        static const int TOTAL_COINS   = 224;
        static const int NORMAL_COIN   = 26;
        static const int SUPER_COIN    = 27;
        static const int NOT_A_COIN    = 0;

        Game_manager(SDL_Renderer* &renderer);
        ~Game_manager();

        void reset_game_state();
        void advance_to_next_level();

        void collect_coin(const int coin_type);
        void defeat_ghost(const int ghost_tile_x , const int ghost_tile_y);
        void decrease_life();

        int get_ghost_streak() const;
        int get_eaten_ghost_pos_x() const;
        int get_eaten_ghost_pos_y() const;
        int get_remaining_lives() const;
        int get_current_level() const;
        int get_menu_selection() const;
        int get_remaining_coins() const;

        bool is_all_coins_collected() const;

        void update_ghost_positions(class Ghost* &pinky , class Ghost* &inky , class Ghost* &clyde , class Ghost* &greendy);

        void render_hud(SDL_Renderer* &renderer);
        void handle_endgame_input(SDL_Event &event , std :: vector <std :: string> &score_data);
        void show_endgame_screen(SDL_Renderer* &renderer);
        void check_score_data(const std :: vector <std :: string> &score_data);

    private:

        int current_level;
        int player_lives;
        int coins_collected;
        int ghosts_defeated;
        int total_score;

        int ghost_pos_x;
        int ghost_pos_y;

        int pinky_coin_trigger;
        int inky_coin_trigger;
        int clyde_coin_trigger;

        int current_button;
        int menu_selection;
        int score_insert_position;

        bool is_new_record = false;

        std :: string player_name;

        Text_manager* life_text;
        Text_manager* score_text;
        Text_manager* level_text;
        Text_manager* player_name_text;

        SDL_Texture* endgame_screen;
        SDL_Texture* highscore_screen;

        Menu_button* confirm_button;
        Menu_button* cancel_button;

        Mix_Chunk* navigation_sound = nullptr;
};

#endif // _GAME_MANAGER_H_
