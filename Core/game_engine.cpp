#include "game_engine.h"
#include <limits>
#include <random>
#include <time.h>
#include <stack>

using namespace std;

Engine :: Engine()
{
    map = nullptr;
    pacman = nullptr;
    blinky = nullptr;
    pinky = nullptr;
    clyde = nullptr;
    inky = nullptr;
    greendy = nullptr;
    friendy = nullptr;
    apple = nullptr;
    object_texture = nullptr;
    tick_manager = nullptr;
    game_manager = nullptr;
    sound_manager = nullptr;
}

Engine :: ~Engine()
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
    delete greendy;
    greendy = nullptr;
    delete friendy;
    friendy = nullptr;
    delete apple;
    apple = nullptr;
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

void Engine :: init(SDL_Renderer* &renderer)
{
    map = new Map();
    object_texture = new Texture_source();
    object_texture -> load_tile_texture(renderer);
    object_texture -> load_pacman_and_ghost_texture(renderer);

    tick_manager = new Tick_manager();
    game_manager = new Game_manager(renderer);
    sound_manager = new Sound_manager();
    sound_manager -> play_sound();

    srand(time(nullptr));
    apple = new Item(renderer , "Source/Assets/Entity Image/greenapple.png");

    new_game();

    SDL_Surface* image = IMG_Load("Source/Assets/Menu Image/nextlevel.png");
    next_level = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
    image = IMG_Load("Source/Assets/Menu Image/ready.png");
    ready = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
}

void Engine :: new_game()
{
    map -> reset_map();
    game_manager -> reset_game_state();

    delete pacman;
    pacman = new Pacman();

    delete blinky;
    if(game_manager -> get_current_level() < 5) {
        blinky = new Ghost(13 , 11 , false);
    } else {
        blinky = new Ghost(12 , 11 , false);
    }

    delete pinky;
    pinky = new Ghost(13 , 14 , true);

    delete inky;
    inky = new Ghost(11 , 14 , true);

    delete clyde;
    clyde = new Ghost(15 , 14 , true);

    if(game_manager -> get_current_level() >= 3) {
        apple -> spawn_at(1 , 1);
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }

    if(game_manager -> get_current_level() >= 5) {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }

    sound_manager -> queue_sound(Sound_manager :: start);
    tick_manager -> reset_tick(game_manager -> get_current_level());
    tick_manager -> toggle_pause(true);
    running_eg_board = false;
}

void Engine :: respawn_object()
{
    delete pacman;
    pacman = new Pacman();

    delete blinky;
    blinky = new Ghost(13 , 11 , false);

    delete pinky;
    pinky = new Ghost(13 , 14 , true);

    delete inky;
    inky = new Ghost(11 , 14 , true);

    delete clyde;
    clyde = new Ghost(15 , 14 , true);

    if(greendy != nullptr) {
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }

    if(friendy != nullptr) {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }

    sound_manager -> clear_status();
    tick_manager -> toggle_pause(false);
}

void Engine :: handle_event(SDL_Event &e , std :: vector<std :: string> &score_data)
{
    if(Mix_Playing(2) || Mix_Playing(4)) return;

    if(pacman -> is_dead()) {
        if(running_eg_board) {
            game_manager -> handle_endgame_input(e , score_data);
        }
        return;
    }

    if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT ||
           e.key.keysym.sym == SDLK_s    || e.key.keysym.sym == SDLK_w  || e.key.keysym.sym == SDLK_a    || e.key.keysym.sym == SDLK_d) {

            int new_dir = -1;
            int last_dir = pacman -> get_current_direction();
            int pacman_tile_x = pacman -> get_tile_x();
            int pacman_tile_y = pacman -> get_tile_y();
            int pacman_pos_x = pacman -> get_pos_x();
            int pacman_pos_y = pacman -> get_pos_y();

            if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) new_dir = Pacman :: up;
            else if(e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) new_dir = Pacman :: right;
            else if(e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) new_dir = Pacman :: down;
            else if(e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) new_dir = Pacman :: left;

            if(last_dir == Pacman :: none_dir) {
                if(map -> can_change_direction(pacman_tile_x , pacman_tile_y , new_dir)) {
                    pacman -> clear_turn_plan();
                    pacman -> handle_input(new_dir);
                }
            }
            else {
                if(new_dir % 2 == last_dir % 2) {
                    if(map -> can_change_direction(pacman_tile_x , pacman_tile_y , new_dir)) {
                        pacman -> handle_input(new_dir);
                        pacman -> clear_turn_plan();
                    }
                }
                else {
                    std :: pair <int , int>  next_cross = map -> get_nxt_cross(pacman_tile_x , pacman_tile_y , last_dir);

                    if(last_dir % 2 == 1 && new_dir % 2 == 0) {
                        if(pacman_pos_y == pacman_tile_y * 16) {
                            if(map -> can_change_direction(pacman_tile_x , pacman_tile_y , new_dir)) {
                                pacman -> plan_turn(new_dir , std :: pair <int , int> (pacman_tile_x , pacman_tile_y));
                            }
                            else if(next_cross != std :: pair <int , int> (-1 , -1) && !map -> beside_cross_is_wall(next_cross , new_dir) && abs(pacman_pos_x - next_cross.first * 16) <= 32) {
                                pacman -> plan_turn(new_dir , next_cross);
                            }
                        }
                    }
                    else if(last_dir % 2 == 0 && new_dir % 2 == 1) {
                        if(pacman_pos_x == pacman_tile_x * 16) {
                            if(map -> can_change_direction(pacman_tile_x , pacman_tile_y , new_dir)) {
                                pacman -> plan_turn(new_dir , std :: pair <int , int> (pacman_tile_x , pacman_tile_y));
                            }
                            else if(next_cross != std :: pair <int , int> (-1 , -1) && !map -> beside_cross_is_wall(next_cross , new_dir) && abs(pacman_pos_y - next_cross.second * 16) <= 32) {
                                pacman -> plan_turn(new_dir , next_cross);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Engine :: render_ghost(SDL_Renderer* &renderer , Ghost* &ghost , int ghost_id)
{
    if(ghost == nullptr) return;

    if(ghost -> is_dead()) {
        object_texture -> render_ghost(
            renderer ,
            ghost -> get_pos_x() ,
            ghost -> get_pos_y() ,
            Texture_source :: ghost_spirit ,
            ghost -> get_ghost_direction()
        );
    }
    else if(ghost -> get_frightened_state()) {
        if(tick_manager ->is_frightened_time() < 2.0) {
            object_texture -> render_ghost(
                renderer ,
                ghost -> get_pos_x() ,
                ghost -> get_pos_y() ,
                ghost_id ,
                Texture_source :: frighten_ghost_2
            );
        } else {
            object_texture -> render_ghost(
                renderer ,
                ghost -> get_pos_x() ,
                ghost -> get_pos_y() ,
                ghost_id ,
                Texture_source :: frighten_ghost_1
            );
        }
    }
    else {
        object_texture -> render_ghost(
            renderer ,
            ghost -> get_pos_x() ,
            ghost -> get_pos_y() ,
            ghost_id ,
            ghost -> get_ghost_direction()
        );
    }
}

void Engine :: render(SDL_Renderer* &renderer , const std :: vector<std :: string> &score_data)
{
    tick_manager -> stabilize_fps();
    SDL_Rect ds_rect;

    for(int i = 0 ; i < 28 ; ++i) {
        for(int j = 0 ; j < 31 ; ++j) {
            ds_rect = {i * 16 + 217 , j * 16 , 16 , 16};
            object_texture -> render_tile_texture(renderer , map -> get_tile(i , j) , &ds_rect);
        }
    }

    if(game_manager -> get_current_level() >= 3 && !apple -> is_destroyed()) {
        apple -> render_item(renderer);
    }

    if(!running_eg_board) {
        int dir = pacman -> get_current_direction();

        if(!pacman -> is_dead()) {
            render_ghost(renderer , blinky , Texture_source :: BLINKY);
            render_ghost(renderer , pinky  , Texture_source :: PINKY);
            render_ghost(renderer , inky   , Texture_source :: INKY);
            render_ghost(renderer , clyde  , Texture_source :: CLYDE);
            if(greendy != nullptr) render_ghost(renderer , greendy , Texture_source :: GREENDY);
            if(friendy != nullptr) render_ghost(renderer , friendy , Texture_source :: FRIENDY);

            if(Mix_Playing(2)) {
                ds_rect = {441 - 82 , 285 - 15 - 7 , 164 , 30};
                SDL_RenderCopy(renderer , ready , nullptr , &ds_rect);
            }
        }

        if(pacman -> is_dead()) {
            if(object_texture -> is_pacman_dead()) {
                if(game_manager -> get_remaining_lives() > 0) {
                    respawn_object();
                } else {
                    running_eg_board = true;
                    game_manager -> check_score_data(score_data);
                }
            } else {
                object_texture -> render_pacman(renderer , pacman -> get_pos_x() , pacman -> get_pos_y() , Texture_source :: dead_pacman);
            }
        } else {
            object_texture -> render_pacman(renderer , pacman -> get_pos_x() , pacman -> get_pos_y() , dir);
        }

        if(wait_time > 0) {
            ds_rect = {441 - 97 , 248 - 52 , 194 , 104};
            SDL_RenderCopy(renderer , next_level , nullptr , &ds_rect);
        }

        if(Mix_Playing(4)) {
            object_texture -> render_ghost_score(
                renderer ,
                game_manager -> get_eaten_ghost_pos_x() ,
                game_manager -> get_eaten_ghost_pos_y() ,
                game_manager -> get_ghost_streak()
            );
        }

        sound_manager -> load_sound();
    }

    if(running_eg_board) {
        game_manager -> show_endgame_screen(renderer);
    }
    else {
        game_manager -> render_hud(renderer);
    }
}

void Engine :: loop(bool &exit_to_menu)
{
    if(game_manager -> is_all_coins_collected()) {
        if(wait_time > 0) --wait_time;
        else {
            game_manager -> advance_to_next_level();
            tick_manager -> reset_tick(game_manager -> get_current_level());
            respawn_object();
            map -> reset_map();
        }
        return;
    }

    if(Mix_Playing(2) || Mix_Playing(4)) {
        if(Mix_Playing(2)) tick_manager -> toggle_pause(true);
        return;
    }

    if(pacman -> is_dead()) {
        if(running_eg_board) {
            switch(game_manager -> get_menu_selection()) {
                case Game_manager :: STATE_AGAIN:
                    new_game();
                    break;
                case Game_manager :: STATE_QUIT:
                    exit_to_menu = true;
                    break;
            }
        }
        return;
    }

    tick_manager -> update_mode_status();

    int tile_x = pacman -> get_tile_x();
    int tile_y = pacman -> get_tile_y();
    int pos_x = pacman -> get_pos_x();
    int pos_y = pacman -> get_pos_y();
    int last_dir = pacman -> get_current_direction();

    if(!pacman -> is_dead() && last_dir != Pacman :: none_dir) {
        if(tile_x * 16 == pos_x && tile_y * 16 == pos_y) {
            if(map -> is_cross_roads(std :: make_pair(tile_x , tile_y))) {
                if(pacman -> has_planned_turn() && pacman -> get_planned_turn_tile() == std :: make_pair(tile_x , tile_y)) {
                    pacman -> perform_planned_turn();
                }
            }

            if(map -> can_change_direction(tile_x , tile_y , pacman -> get_current_direction())) {
                pacman -> update();
            } else {
                pacman -> stop_moving();
            }
        }
        else {
            if(map -> can_change_direction(tile_x , tile_y , last_dir)) {
                pacman -> update();
            } else {
                if((tile_x * 16 == pos_x && tile_y * 16 != pos_y) || (tile_x * 16 != pos_x && tile_y * 16 == pos_y)) {
                    pacman -> update();
                } else {
                    pacman -> stop_moving();
                }
            }
        }
    }

    int remain_coin = game_manager -> get_remaining_coins();
    switch(remain_coin / 50) {
        case 0:
            sound_manager -> queue_sound(Sound_manager :: step_3);
            break;
        case 1:
            sound_manager -> queue_sound(Sound_manager :: step_2);
            break;
        case 2:
            sound_manager -> queue_sound(Sound_manager :: step_1);
            break;
        default:
            sound_manager -> queue_sound(Sound_manager :: step_0);
            break;
    }

    tile_x = pacman -> get_tile_x();
    tile_y = pacman -> get_tile_y();
    int coin_type = map -> eat_coins(tile_x , tile_y);

    if(coin_type != Game_manager :: NOT_A_COIN) {
        game_manager -> collect_coin(coin_type);
        sound_manager -> queue_sound(Sound_manager :: chomp);

        if(coin_type == Game_manager :: SUPER_COIN) {
            tick_manager -> activate_frightened_mode();
            sound_manager -> queue_sound(Sound_manager :: ghost_turn_blue);

            if(!blinky -> is_dead()) blinky -> set_frightened_mode(true);
            if(!pinky  -> is_dead()) pinky  -> set_frightened_mode(true);
            if(!inky   -> is_dead()) inky   -> set_frightened_mode(true);
            if(!clyde  -> is_dead()) clyde  -> set_frightened_mode(true);

            if(greendy != nullptr && !greendy -> is_dead()) greendy -> set_frightened_mode(true);

            if(friendy != nullptr) {
                tick_manager -> start_friendy_chase();
            }
        }
    }

    if(!tick_manager -> is_frightened_time()) {
        sound_manager -> queue_sound(Sound_manager :: normal_ghost);
        blinky -> set_frightened_mode(false);
        pinky  -> set_frightened_mode(false);
        inky   -> set_frightened_mode(false);
        clyde  -> set_frightened_mode(false);
        if(greendy != nullptr) greendy -> set_frightened_mode(false);
    }

    bool scatter = tick_manager -> is_scatter_time();
    blinky -> set_scatter_mode(scatter);
    pinky  -> set_scatter_mode(scatter);
    inky   -> set_scatter_mode(scatter);
    clyde  -> set_scatter_mode(scatter);

    if(!tick_manager -> is_greendy_chase_time()) {
        eat_green_apple = false;
    } else {
        if(apple -> is_destroyed()) {
            switch(rand() % 4) {
                case 0: apple -> spawn_at(1 , 1); break;
                case 1: apple -> spawn_at(26 , 1); break;
                case 2: apple -> spawn_at(26 , 29); break;
                case 3: apple -> spawn_at(1 , 29); break;
            }
        }
    }

    pos_x = pacman -> get_pos_x();
    pos_y = pacman -> get_pos_y();

    if(!pacman -> is_dead()) {
        tick_manager -> toggle_pause (false);
    }

    pacman -> go_through_portal();

    ghost_move(blinky);
    ghost_move(pinky);
    ghost_move(inky);
    ghost_move(clyde);
    ghost_move(greendy);
    ghost_move(friendy);

    game_manager -> update_ghost_positions(pinky , inky , clyde , greendy);

    if(game_manager -> is_all_coins_collected()) {
        sound_manager -> queue_sound(Sound_manager :: win_level);
        wait_time = 100;
    }
}

// ... toàn bộ phần trước giữ nguyên

void Engine :: ghost_move(Ghost* &ghost)
{
    if(ghost == nullptr) return;

    int ghost_tile_x = ghost -> get_tile_x();
    int ghost_tile_y = ghost -> get_tile_y();
    int ghost_pos_x = ghost -> get_pos_x();
    int ghost_pos_y = ghost -> get_pos_y();
    int ghost_old_dir = ghost -> get_ghost_direction();
    int ghost_next_tile_x = ghost -> get_target_tile_x();
    int ghost_next_tile_y = ghost -> get_target_tile_y();

    if(ghost_tile_x * 16 == ghost_pos_x && ghost_tile_y * 16 == ghost_pos_y) {
        if(map -> is_cross_roads(std :: make_pair(ghost_tile_x , ghost_tile_y))) {
            if(ghost -> get_frightened_state() || (ghost == friendy && !tick_manager -> is_friendy_chase_time())) {
                std :: stack <int> which_dir;
                if(ghost_old_dir % 2 == 1) {
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: up)) which_dir.push(0);
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: down)) which_dir.push(2);
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , ghost_old_dir)) which_dir.push(ghost_old_dir);
                } else {
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: left)) which_dir.push(3);
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: right)) which_dir.push(1);
                    if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , ghost_old_dir)) which_dir.push(ghost_old_dir);
                }
                int dir = rand() % ((int) which_dir.size()) + 1;
                while(dir > 1) which_dir.pop() , --dir;
                ghost -> set_direction(which_dir.top());
                while(!which_dir.empty()) which_dir.pop();
            } else {
                int distance_up = __INT32_MAX__;
                int distance_down = __INT32_MAX__;
                int distance_left = __INT32_MAX__;
                int distance_right = __INT32_MAX__;

                if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: up))
                    distance_up = map -> get_distance({ghost_tile_x , ghost_tile_y - 1} , {ghost_next_tile_x , ghost_next_tile_y} , Map :: up);

                if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: down))
                    distance_down = map -> get_distance({ghost_tile_x , ghost_tile_y + 1} , {ghost_next_tile_x , ghost_next_tile_y} , Map :: down);

                if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: left))
                    distance_left = map -> get_distance({ghost_tile_x - 1 , ghost_tile_y} , {ghost_next_tile_x , ghost_next_tile_y} , Map :: left);

                if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , Map :: right))
                    distance_right = map -> get_distance({ghost_tile_x + 1 , ghost_tile_y} , {ghost_next_tile_x , ghost_next_tile_y} , Map :: right);

                int distance_min;
                if(ghost_old_dir == Map :: up) {
                    distance_min = std :: min(distance_up , std :: min(distance_left , distance_right));
                    if(distance_min == distance_up) ghost -> set_direction(Map :: up);
                    else if(distance_min == distance_left) ghost -> set_direction(Map :: left);
                    else ghost -> set_direction(Map :: right);
                } else if(ghost_old_dir == Map :: down) {
                    distance_min = std :: min(distance_down , std :: min(distance_left , distance_right));
                    if(distance_min == distance_down) ghost -> set_direction(Map :: down);
                    else if(distance_min == distance_left) ghost -> set_direction(Map :: left);
                    else ghost -> set_direction(Map :: right);
                } else if(ghost_old_dir == Map :: left) {
                    distance_min = std :: min(distance_up , std :: min(distance_down , distance_left));
                    if(distance_min == distance_up) ghost -> set_direction(Map :: up);
                    else if(distance_min == distance_down) ghost -> set_direction(Map :: down);
                    else ghost -> set_direction(Map :: left);
                } else if(ghost_old_dir == Map :: right) {
                    distance_min = std :: min(distance_up , std :: min(distance_right , distance_down));
                    if(distance_min == distance_up) ghost -> set_direction(Map :: up);
                    else if(distance_min == distance_right) ghost -> set_direction(Map :: right);
                    else ghost -> set_direction(Map :: down);
                }
            }
        }
        if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , ghost -> get_ghost_direction())) ghost -> move();
    } else {
        if(map -> can_change_direction(ghost_tile_x , ghost_tile_y , ghost -> get_ghost_direction())) ghost -> move();
        else {
            if(ghost_tile_x * 16 == ghost_pos_x && ghost_tile_y * 16 != ghost_pos_y && ghost -> get_ghost_direction() % 2 == 0) ghost -> move();
            else if(ghost_tile_y * 16 == ghost_pos_y && ghost_tile_x * 16 != ghost_pos_x && ghost -> get_ghost_direction() % 2 == 1) ghost -> move();
        }
    }

    ghost -> go_through_portal();
    if(ghost_pos_x == ghost_next_tile_x * 16 && ghost_pos_y == ghost_next_tile_y * 16) {
        if(ghost -> is_dead()) {
            ghost -> set_dead(false , "ghost");
            sound_manager -> queue_sound(Sound_manager :: revive_ghost);
        } else {
            if(ghost == greendy) {
                tick_manager -> start_greendy_chase();
                ghost -> reset_object_tile(ghost_tile_x , ghost_tile_y);
                apple -> destroy();
                eat_green_apple = true;
            }
        }
    }
    pacman_meat_ghost(ghost);
}
