#include "game_manager.h"
#include <iostream>
#include <string>

GameManager :: GameManager(SDL_Renderer* &renderer)
{
    level = 1;
    life = 5;
    eaten_coins = 0;
    eaten_ghosts = -1;
    scores = 0;
    player_name = "Unknown";
    player_decision = WAITING;
    current_button = 1;
    PINKY_COIN_LIMIT = 5;
    INKY_COIN_LIMIT = 30;
    CLYDE_COIN_LIMIT = 90;

    live_text = new TextManager(28);
    live_text -> load_render_text(renderer  , "Lives:" , {255 , 255 , 255 , 255});
    score_text = new TextManager(28);
    score_text -> load_render_text(renderer , "Scores: 0" , {255 , 255 , 255 , 255});
    level_text = new TextManager(28);
    level_text -> load_render_text(renderer , "Level: 1" , {255 , 255 , 255 , 255});
    player_NAME = new TextManager(20);
    player_NAME -> load_render_text(renderer , player_name.c_str() , {255 , 255 , 255 , 255});

    endgame_board = load_image(renderer , "Assets/Menu Image/endgame.png");
    highscores_board = load_image(renderer , "Assets/Menu Image/newHighscore.png");

    button_yes = new Button(70 , 30 , 478 , 250);
    button_yes -> load_button(renderer , "Yes");
    button_yes -> set_status(Button :: BUTTON_IN);
    button_no = new Button(70 , 30 , 580 , 250);
    button_no -> load_button(renderer , "No");
    button_no -> set_status(Button :: BUTTON_OUT);
}

GameManager :: ~GameManager()
{
    delete level_text;
    level_text = nullptr;

    delete live_text;
    live_text = nullptr;

    delete score_text;
    score_text = nullptr;

    delete endgame_board;
    endgame_board = nullptr;

    delete highscores_board;
    highscores_board = nullptr;

    delete player_NAME;
    player_NAME = nullptr;

    Mix_FreeChunk(navigation_sound);
}

SDL_Texture* GameManager :: load_image(SDL_Renderer* &renderer , const std :: string image_path)
{
    SDL_Surface* image = IMG_Load(image_path.c_str());
    SDL_Texture* load_texture = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
    return load_texture;
}

void GameManager :: reset_game()
{
    level = 1;
    life = 5;
    scores = 0;
    eaten_coins = 0;
    eaten_ghosts = -1;
    current_button = 1;
    pos = -1;
    PINKY_COIN_LIMIT = 5;
    INKY_COIN_LIMIT = 30;
    CLYDE_COIN_LIMIT = 90;
    player_decision = WAITING;
    button_yes -> set_status(Button :: BUTTON_IN);
    button_no -> set_status(Button :: BUTTON_OUT);
}

void GameManager :: eat_coin(const int type_of_coin)
{
    eaten_coins++;
    if(type_of_coin == NORMAL_COIN) scores += 10;
    else if(type_of_coin == SUPER_COIN){
        eaten_ghosts = -1;
        scores += 50;
    }
}

void GameManager :: eat_ghost(const int ghost_tile_col , const int ghost_tile_row)
{
    eaten_ghosts = eaten_ghosts + 1;
    if(eaten_ghosts == 0) {
        scores += 200;
    }
    if(eaten_ghosts == 1) {
        scores += 400;
    }
    if(eaten_ghosts == 2) {
        scores += 800;
    }
    if(eaten_ghosts == 3) {
        scores += 1600;
    }
    if(eaten_ghosts > 3) {
        scores += 2000;
    }
    ghost_is_eaten_in_pos_col = ghost_tile_col;
    ghost_is_eaten_in_pos_row = ghost_tile_row;
}

void GameManager :: lost_a_life()
{
    life--;
}

void GameManager :: next_level()
{
    level++;
    if(level % 5 == 0) {
        life = 10;
    }
    else life = 5;
    eaten_coins = 0;
    eaten_ghosts = -1;
    current_button = 1;
    player_decision = WAITING;
    if(level <= 3) {
        PINKY_COIN_LIMIT = 5;
        INKY_COIN_LIMIT = 30;
        CLYDE_COIN_LIMIT = 90;
    }
    else if(level <= 5) {
        PINKY_COIN_LIMIT = 0;
        INKY_COIN_LIMIT = 5;
        CLYDE_COIN_LIMIT = 10;
    }
    else PINKY_COIN_LIMIT = INKY_COIN_LIMIT = CLYDE_COIN_LIMIT = 0;
}

void GameManager :: handle_ghost_pos(Ghost* &pinky , Ghost* &inky , Ghost* &clyde , Ghost* &greendy)
{
    if(pinky -> is_ghost_in_cage() && eaten_coins >= PINKY_COIN_LIMIT) pinky -> ghost_respawn(Ghost :: GHOST_START_TILE_COL , Ghost :: GHOST_START_TILE_ROW , false);
    if(inky  -> is_ghost_in_cage() && eaten_coins >=  INKY_COIN_LIMIT) inky  -> ghost_respawn(Ghost :: GHOST_START_TILE_COL , Ghost :: GHOST_START_TILE_ROW , false);
    if(clyde -> is_ghost_in_cage() && eaten_coins >= CLYDE_COIN_LIMIT) clyde -> ghost_respawn(Ghost :: GHOST_START_TILE_COL , Ghost :: GHOST_START_TILE_ROW , false);
    if(get_remain_coins() < 100 && greendy != nullptr && greendy -> is_ghost_in_cage()) greendy -> ghost_respawn(Ghost :: GHOST_START_TILE_COL , Ghost :: GHOST_START_TILE_ROW , false);
}

void GameManager :: render_HUD(SDL_Renderer* &renderer)
{
    level_text -> load_render_text(renderer , "Level: " + std :: to_string(level) , {255 , 255 , 255 , 255});
    level_text -> render_text(renderer , 0 , 0 , TextManager :: LEFT);
    live_text -> load_render_text(renderer , "Life: " + std :: to_string(life) , {255 , 255 , 255 , 255});
    live_text -> render_text(renderer , 0 , 50 , TextManager :: LEFT);
    score_text -> load_render_text(renderer , "Scores: " + std :: to_string(scores) , {255 , 255 , 255 , 255});
    score_text -> render_text(renderer , 0 , 100 , TextManager :: LEFT);
}

void GameManager :: run_endgame_board(SDL_Renderer* &renderer)
{
    SDL_Rect display_rect = {441 - 250 , 248 - 150 , 500 , 300};
    SDL_RenderCopy(renderer , endgame_board , nullptr , &display_rect);
    button_yes -> render_button(renderer);
    button_no  -> render_button(renderer);
    if(new_record) {
        SDL_RenderCopy(renderer , highscores_board , nullptr , &display_rect);
        static int caret_time = 0;
        SDL_Rect caret = {395 + player_NAME -> get_text_length() , 265 , 2 , 20};
        if(caret_time % 20 > 10) SDL_RenderFillRect(renderer , &caret);
        caret_time++;
        caret_time %= 20;
        if(player_name != "") {
            player_NAME -> load_render_text(renderer , player_name.c_str() , {0 , 0 , 0 , 255});
            player_NAME -> render_text(renderer , 395 , 268 , TextManager :: LEFT);
        }
    }
}

void GameManager :: handle_endgame_board(SDL_Event &event , std :: vector <std :: string> &score_data)
{
    if(new_record){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RETURN && player_name.length() > 2){
                Mix_PlayChannel(7, navigation_sound, 0);
                SDL_StopTextInput();
                std :: string data = player_name + ": " + std :: to_string(scores);
                score_data.emplace(score_data.begin() + pos, data.c_str());
                score_data.pop_back();
                new_record = false;
            }
            if(event.key.keysym.sym == SDLK_BACKSPACE && player_name.length() > 0){
                player_name.pop_back();
            }
            else if(event.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)){
                SDL_SetClipboardText(player_name.c_str());
            }
            else if(event.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL)){
                player_name = SDL_GetClipboardText();
            }
        }
        else if(event.type == SDL_TEXTINPUT) {
            if(!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')) && player_name.length() < 22){
                if((event.text.text[0] >= 'a' && event.text.text[0] <= 'z') || (event.text.text[0] >= 'A' && event.text.text[0] <= 'Z') || (event.text.text[0] >= '0' && event.text.text[0] <= '9') || event.text.text[0] == ' ')
                    player_name += event.text.text;
            }
        }
    }
    else{
        if(event.type == SDL_KEYDOWN){
            Mix_PlayChannel(7, navigation_sound, 0);
            if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT){
                current_button = 2;
                button_no -> set_status(Button :: BUTTON_IN);
                button_yes -> set_status(Button :: BUTTON_OUT);
            }
            else if(event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT){
                current_button = 1;
                button_yes -> set_status(Button :: BUTTON_IN);
                button_no -> set_status(Button :: BUTTON_OUT);
            }
            else if(event.key.keysym.sym == SDLK_RETURN){
                if(current_button == 1) player_decision = AGAIN;
                else player_decision = QUIT;
            }
            return;
        }
    }
}

void GameManager :: check_score_data(const std :: vector <std :: string> &score_data)
{
    for(int i = 0;i < score_data.size();++i){
        int t = 0 , j = 0;
        while(j < score_data[i].length() && score_data[i][j] != ':') ++j;
        j += 2;
        while(j < score_data[i].length()) t = t * 10 + score_data[i][j] - '0' , ++j;
        if(scores > t){
            new_record = true;
            SDL_StartTextInput();
            pos = i;
            break;
        }
    }
}

int GameManager :: get_eaten_ghost_streak() const { return eaten_ghosts; }
int GameManager :: get_eaten_ghost_pos_col() const { return ghost_is_eaten_in_pos_col; }
int GameManager :: get_eaten_ghost_pos_row() const { return ghost_is_eaten_in_pos_row; }
int GameManager :: get_remain_life() const { return life; }
int GameManager :: get_level() const { return level; }
int GameManager :: get_player_decision() const { return player_decision; }
int GameManager :: get_remain_coins() const { return TOTAL_COINS - eaten_coins; }
bool GameManager :: eat_all_coins() const { return eaten_coins == TOTAL_COINS; }
