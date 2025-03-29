#include "game_manager.h"
#include "../Object/ghost.h"
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <sstream>

Game_manager :: Game_manager(SDL_Renderer* &renderer)
{
    current_level = 1;
    player_lives = 3;
    coins_collected = 0;
    ghosts_defeated = -1;
    total_score = 0;
    player_name = "Unknown";
    menu_selection = STATE_WAITING;
    current_button = 1;

    pinky_coin_trigger = 5;
    inky_coin_trigger = 30;
    clyde_coin_trigger = 90;

    life_text = new Text_manager(28);
    score_text = new Text_manager(28);
    level_text = new Text_manager(28);
    player_name_text = new Text_manager(20);

    life_text -> load_render_text(renderer , "Lives:" , {255 , 255 , 255 , 255});
    score_text -> load_render_text(renderer , "Scores: 0" , {255 , 255 , 255 , 255});
    level_text -> load_render_text(renderer , "Level: 1" , {255 , 255 , 255 , 255});
    player_name_text -> load_render_text(renderer , player_name.c_str() , {255 , 255 , 255 , 255});

    endgame_screen = IMG_LoadTexture(renderer , "Source/Assets/Menu Image/endgame.png");
    highscore_screen = IMG_LoadTexture(renderer , "Source/Assets/Menu Image/newHighscore.png");

    confirm_button = new Menu_button(70 , 30 , 478 , 250);
    confirm_button -> load_label(renderer , "Yes");
    confirm_button -> set_state(Menu_button :: STATE_HOVERED);

    cancel_button = new Menu_button(70 , 30 , 580 , 250);
    cancel_button -> load_label(renderer , "No");
    cancel_button -> set_state(Menu_button :: STATE_NORMAL);

    navigation_sound = Mix_LoadWAV("Source/Sound/navigation.wav");
}

Game_manager :: ~Game_manager()
{
    delete life_text;
    delete score_text;
    delete level_text;
    delete player_name_text;
    delete confirm_button;
    delete cancel_button;

    SDL_DestroyTexture(endgame_screen);
    SDL_DestroyTexture(highscore_screen);

    Mix_FreeChunk(navigation_sound);

    life_text = nullptr;
    score_text = nullptr;
    level_text = nullptr;
    player_name_text = nullptr;
    confirm_button = nullptr;
    cancel_button = nullptr;
    endgame_screen = nullptr;
    highscore_screen = nullptr;
    navigation_sound = nullptr;
}

void Game_manager :: reset_game_state()
{
    current_level = 1;
    player_lives = 3;
    coins_collected = 0;
    ghosts_defeated = -1;
    total_score = 0;
    menu_selection = STATE_WAITING;
    current_button = 1;
    is_new_record = false;
    score_insert_position = -1;
}

void Game_manager :: advance_to_next_level()
{
    current_level++;
    coins_collected = 0;
    ghosts_defeated = -1;
    current_button = 1;
    menu_selection = STATE_WAITING;

    if(current_level <= 3) {
        pinky_coin_trigger = 5;
        inky_coin_trigger = 30;
        clyde_coin_trigger = 90;
    }
    else if(current_level <= 5) {
        pinky_coin_trigger = 0;
        inky_coin_trigger  = 5;
        clyde_coin_trigger = 10;
    }
    else {
        pinky_coin_trigger = inky_coin_trigger = clyde_coin_trigger = 0;
    }
}

void Game_manager :: collect_coin(const int coin_type)
{
    coins_collected++;
    if(coin_type == NORMAL_COIN) total_score += 10;
    else if(coin_type == SUPER_COIN) {
        ghosts_defeated = -1;
        total_score += 50;
    }
}

void Game_manager :: defeat_ghost(const int ghost_tile_x , const int ghost_tile_y)
{
    (++ghosts_defeated) %= 4;
    ghost_pos_x = ghost_tile_x;
    ghost_pos_y = ghost_tile_y;

    switch(ghosts_defeated) {
        case 0: total_score += 200; break;
        case 1: total_score += 400; break;
        case 2: total_score += 800; break;
        case 3: total_score += 1600; break;
    }
}

void Game_manager :: decrease_life()
{
    player_lives--;
}

int Game_manager :: get_ghost_streak() const { return ghosts_defeated; }
int Game_manager :: get_eaten_ghost_pos_x() const { return ghost_pos_x; }
int Game_manager :: get_eaten_ghost_pos_y() const { return ghost_pos_y; }
int Game_manager :: get_remaining_lives() const { return player_lives; }
int Game_manager :: get_current_level() const { return current_level; }
int Game_manager :: get_menu_selection() const { return menu_selection; }
int Game_manager :: get_remaining_coins() const { return TOTAL_COINS - coins_collected; }
bool Game_manager :: is_all_coins_collected() const { return coins_collected == TOTAL_COINS; }

void Game_manager :: update_ghost_positions(Ghost* &pinky , Ghost* &inky , Ghost* &clyde , Ghost* &greendy)
{
    if(pinky -> inside_cage() && coins_collected >= pinky_coin_trigger)
        pinky -> ghost_respawn(Ghost :: ghost_start_tile_x , Ghost :: ghost_start_tile_y , false);

    if(inky -> inside_cage() && coins_collected >= inky_coin_trigger)
        inky -> ghost_respawn(Ghost :: ghost_start_tile_x , Ghost :: ghost_start_tile_y , false);

    if(clyde -> inside_cage() && coins_collected >= clyde_coin_trigger)
        clyde -> ghost_respawn(Ghost :: ghost_start_tile_x , Ghost :: ghost_start_tile_y , false);

    if(get_remaining_coins() < 100 && greendy != nullptr && greendy -> inside_cage())
        greendy -> ghost_respawn(Ghost :: ghost_start_tile_x , Ghost :: ghost_start_tile_y , false);
}

void Game_manager :: render_hud(SDL_Renderer* &renderer)
{
    level_text -> load_render_text(renderer , "Level: " + std :: to_string(current_level) , {255 , 255 , 255 , 255});
    level_text -> render_text(renderer , 0 , 0 , Text_manager :: LEFT);

    life_text -> load_render_text(renderer , "Life: " + std :: to_string(player_lives) , {255 , 255 , 255 , 255});
    life_text -> render_text(renderer , 0 , 50 , Text_manager :: LEFT);

    score_text -> load_render_text(renderer , "Scores: " + std :: to_string(total_score) , {255 , 255 , 255 , 255});
    score_text -> render_text(renderer , 0 , 100 , Text_manager :: LEFT);
}

void Game_manager :: handle_endgame_input(SDL_Event &e , std :: vector<std :: string> &score_data)
{
    if(e.type == SDL_KEYDOWN) {
        Mix_PlayChannel(-1 , navigation_sound , 0);

        switch(e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                current_button = 1;
                confirm_button -> set_state(Menu_button :: STATE_HOVERED);
                cancel_button -> set_state(Menu_button :: STATE_NORMAL);
                break;

            case SDLK_RIGHT:
            case SDLK_d:
                current_button = 2;
                confirm_button -> set_state(Menu_button :: STATE_NORMAL);
                cancel_button -> set_state(Menu_button :: STATE_HOVERED);
                break;

            case SDLK_RETURN:
                menu_selection = (current_button == 1) ? STATE_AGAIN : STATE_QUIT;
                break;
        }
    }
}

void Game_manager :: show_endgame_screen(SDL_Renderer* &renderer)
{
    SDL_Rect screen_rect = {448 , 0 , 224 , 496};
    SDL_RenderCopy(renderer , endgame_screen , nullptr , &screen_rect);
    confirm_button -> button_render(renderer);
    cancel_button -> button_render(renderer);

    if(is_new_record) {
        SDL_RenderCopy(renderer , highscore_screen , nullptr , &screen_rect);
        SDL_Rect name_rect = {395 , 268 , player_name_text -> get_text_width() , 30};
        player_name_text -> render_text(renderer , name_rect.x , name_rect.y , Text_manager :: LEFT);
    }
}

void Game_manager :: check_score_data(const std :: vector<std :: string> &score_data)
{
    for(int i = 0; i < score_data.size(); i++) {
        int extracted_score = 0;
        int j = score_data[i].find_last_of(':') + 2;
        while(j < score_data[i].size()) extracted_score = extracted_score * 10 + (score_data[i][j++] - '0');
        if(total_score > extracted_score) {
            is_new_record = true;
            score_insert_position = i;
            SDL_StartTextInput();
            break;
        }
    }
}
