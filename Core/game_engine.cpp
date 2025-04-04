#include "game_engine.h"
#include <limits>
#include <random>
#include <time.h>

using namespace std;

typedef std :: pair <int , int> II;
typedef std :: pair <int , std :: pair <int , int> > IP;

void Engine :: init_game(SDL_Renderer* &renderer)
{
    map = new Map();

    object_texture = new TextureSrc();
    object_texture -> load_tile_texture(renderer);
    object_texture -> load_pacman_and_ghost_texture(renderer);

    tick_manager = new TickManager();
    game_manager = new GameManager(renderer);
    sound_manager = new SoundManager();
    sound_manager -> load_sound();

    srand(time(nullptr));
    apple = new Item(renderer , "Assets/Entity Image/greenapple.png");
    new_game();

    SDL_Surface* image = IMG_Load("Assets/Menu Image/nextlevel.png");
    next_level = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);

    image = IMG_Load("Assets/Menu Image/ready.png");
    ready = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
}

void Engine :: new_game()
{
    map -> map_reset();
    game_manager -> reset_game();

    delete pacman;
    pacman = new Pacman();

    delete blinky;
    if(game_manager -> get_level() < 5) blinky = new Ghost(13 , 11 , false);
    else blinky = new Ghost(12 , 11 , false);

    delete pinky;
    pinky = new Ghost(13 , 14 , true);

    delete inky;
    inky = new Ghost(11 , 14 , true);

    delete clyde;
    clyde = new Ghost(15 , 14 , true);

    if(game_manager -> get_level() >= 1)
    {
        apple -> spawn_at(1 , 1);
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }
    if(game_manager -> get_level() >= 2)
    {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }

    sound_manager -> insert_playlist(SoundManager :: START);
    tick_manager -> reset_tick(game_manager -> get_level());
    tick_manager -> pause_tick(true);
    running_endgame_board = false;
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
    if(greendy != nullptr)
    {
        delete greendy;
        greendy = new Ghost(12 , 15 , true);
    }
    if(friendy != nullptr)
    {
        delete friendy;
        friendy = new Ghost(14 , 11 , false);
    }
    sound_manager -> reset_sound();
    tick_manager -> pause_tick(false);
}

void Engine :: handle_event_engine(SDL_Event &event , std :: vector <std :: string> &score_data)
{
    if(Mix_Playing(2) || Mix_Playing(4)) return; // nếu đang phát start/dead/eat_ghost thì return
    if(pacman -> is_dead()) // nếu pacman bị ghost kill
    {
        if(running_endgame_board) {
            game_manager -> handle_endgame_board(event , score_data);
        }
        return;
    }
    if(event.type == SDL_KEYDOWN)
    {
        if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT ||
           event.key.keysym.sym == SDLK_s    || event.key.keysym.sym == SDLK_w  || event.key.keysym.sym == SDLK_a    || event.key.keysym.sym == SDLK_d    )
        {
            int new_direction = -1; // hướng người chơi vừa nhấn
            int last_direction = -1; // hướng pacman đang đi hiện tại
            int pacman_tile_col = pacman -> get_tile_col();
            int pacman_tile_row = pacman -> get_tile_row();
            int pacman_screen_pos_col = pacman -> get_screen_pos_col();
            int pacman_screen_pos_row = pacman -> get_screen_pos_row();
            if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();
            if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) new_direction = UP;
            else if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) new_direction = RIGHT;
            else if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) new_direction = DOWN;
            else if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) new_direction = LEFT;
            if(last_direction == -1)
            {
                if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction))
                {
                    pacman -> clear_special();
                    pacman -> push_to_direction(new_direction);
                }
            }
            else
            {
                if(new_direction % 2 == last_direction % 2)
                {
                    if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction))
                    {
                        pacman -> push_to_direction(new_direction);
                        pacman -> clear_special();
                    }
                }
                else
                {
                    std :: pair <int , int> next_cross_id = map -> get_nearest_cross_tile_id(pacman_tile_col , pacman_tile_row , last_direction);
                    if(last_direction % 2 == 1 && new_direction % 2 == 0)
                    {
                        if(pacman_screen_pos_row == pacman_tile_row * TILE_SIZE)
                        {
                            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction)) {
                                pacman -> push_to_special(new_direction , II(pacman_tile_col , pacman_tile_row));
                            }
                            else if(next_cross_id != II(-1 , -1) && !map -> is_beside_cross_is_wall(next_cross_id , new_direction) && abs(pacman_screen_pos_col - next_cross_id.first * TILE_SIZE) <= 32) {
                                pacman -> push_to_special(new_direction , next_cross_id);
                            }
                        }
                    }
                    else if(last_direction % 2 == 0 && new_direction % 2 == 1)
                    {
                        if(pacman_screen_pos_col == pacman_tile_col * TILE_SIZE)
                        {
                            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction))
                                pacman -> push_to_special(new_direction , II(pacman_tile_col , pacman_tile_row));
                            else if(next_cross_id != II(-1 , -1) && !map -> is_beside_cross_is_wall(next_cross_id , new_direction) && abs(pacman_screen_pos_row - next_cross_id.second * TILE_SIZE) <= 32)
                                pacman -> push_to_special(new_direction , next_cross_id);
                        }
                    }
                }
            }
        }
    }
}

void Engine :: render_game(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data)
{
    tick_manager -> stablize_FPS();
    SDL_Rect dsRect;
    for(int i = 0; i < 28; ++i)
    {
        for(int j = 0; j < 31; ++j)
        {
            dsRect = {i * TILE_SIZE + 217 , j * TILE_SIZE , TILE_SIZE , TILE_SIZE};
            object_texture -> render_tile_texture(renderer , map -> get_tile_id(i , j) , &dsRect);
        }
    }
    if(game_manager -> get_level() >= 1 && !apple -> is_destroyed()) apple -> render_item(renderer);

    if(!running_endgame_board)
    {
        int dir = -1;
        if(!pacman -> is_direction_empty()) dir = pacman -> get_pacman_direction();
        if(!pacman -> is_dead())
        {
            render_ghost(renderer , blinky , TextureSrc :: BLINKY);
            render_ghost(renderer , pinky , TextureSrc :: PINKY);
            render_ghost(renderer , inky , TextureSrc :: INKY);
            render_ghost(renderer , clyde , TextureSrc :: CLYDE);
            if(greendy != nullptr) render_ghost(renderer , greendy , TextureSrc :: GREENDY);
            if(friendy != nullptr) render_ghost(renderer , friendy , TextureSrc :: FRIENDY);
            if(Mix_Playing(2))
            {
                dsRect = {441 - 82 , 285 - 15 - 7 , 164 , 30};
                SDL_RenderCopy(renderer , ready , nullptr , &dsRect);
            }
        }
        if(pacman -> is_dead())
        {
            if(object_texture -> is_pacman_dead())
            {
                if(game_manager -> get_remain_life() > 0) respawn_object();
                else
                {
                    running_endgame_board = true;
                    game_manager -> check_score_data(score_data);
                }
            }
            else object_texture -> render_pacman_texture(renderer , pacman -> get_screen_pos_col() , pacman -> get_screen_pos_row() , TextureSrc :: DEAD_PACMAN);
        }
        else object_texture -> render_pacman_texture(renderer , pacman -> get_screen_pos_col() , pacman -> get_screen_pos_row() , dir);
        if(wait_time > 0)
        {
            dsRect = {441 - 97 , 248 - 52 , 194 , 104};
            SDL_RenderCopy(renderer , next_level , nullptr , &dsRect);
        }
        if(Mix_Playing(4))
        {
            object_texture -> render_ghost_score(renderer , game_manager -> get_eaten_ghost_pos_col() , game_manager -> get_eaten_ghost_pos_row() , game_manager -> get_eaten_ghost_streak());
        }
        sound_manager -> play_sound();
    }

    if(running_endgame_board) game_manager -> run_endgame_board(renderer);
    else game_manager -> render_HUD(renderer);
}

void Engine :: pacman_meat_ghost(Ghost* &ghost)
{
    if(ghost -> is_dead()) return;
    int distance = (pacman -> get_screen_pos_col() - ghost -> get_screen_pos_col()) * (pacman -> get_screen_pos_col() - ghost -> get_screen_pos_col()) +
                   (pacman -> get_screen_pos_row() - ghost -> get_screen_pos_row()) * (pacman -> get_screen_pos_row() - ghost -> get_screen_pos_row());
    if(distance <= 9)
    {
        if(ghost -> is_ghost_frighten())
        {
            game_manager -> eat_ghost(ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row());
            ghost -> set_dead(true);
            ghost -> set_ghost_frighten(false);
            sound_manager -> insert_playlist(SoundManager :: EAT_GHOST);
            sound_manager -> insert_playlist(SoundManager :: GHOST_GO_HOME);
        }
        else
        {
            pacman -> set_dead(true , 1);
            game_manager -> lost_a_life();
            sound_manager -> insert_playlist(SoundManager :: DEAD);
            tick_manager -> pause_tick(true);
        }
    }
}

void Engine :: render_ghost(SDL_Renderer* &renderer , Ghost* &ghost , int ghost_id)
{
    if(ghost == nullptr) return;
    if(ghost -> is_dead())
    {
        object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , TextureSrc :: GHOST_SPIRIT , ghost -> get_ghost_direction());
    }
    else if(ghost -> is_ghost_frighten())
    {
        if(tick_manager -> remain_frighten_time() < 2.0)
        {
            object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , TextureSrc :: FRIGHTEN_GHOST_2);
        }
        else
        {
            object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , TextureSrc :: FRIGHTEN_GHOST_1);
        }
    }
    else
    {
        object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , ghost -> get_ghost_direction());
    }
}

void Engine :: ghost_move(Ghost* &ghost)
{
    if(ghost == nullptr) return;
    int ghostTileX = ghost -> get_tile_col();
    int ghostTileY = ghost -> get_tile_row();
    int ghostPosX = ghost -> get_screen_pos_col();
    int ghostPosY = ghost -> get_screen_pos_row();
    int ghostOldDir = ghost -> get_ghost_direction();
    int ghostNextTileX = ghost -> get_target_tile_col();
    int ghostNextTileY = ghost -> get_target_tile_row();

    if(ghostTileX * TILE_SIZE == ghostPosX && ghostTileY * TILE_SIZE == ghostPosY)
    {
        if(map -> is_tile_cross(ghostTileX , ghostTileY))
        {
            if(ghost -> is_ghost_frighten() || (ghost == friendy && !tick_manager -> is_friendy_chase_time()))
            {
                std :: stack<int> whichDir;
                if(ghostOldDir % 2 == 1)
                {
                    if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: UP)) whichDir.push(0);
                    if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: DOWN)) whichDir.push(2);
                    if(map -> can_change_direction(ghostTileX , ghostTileY , ghostOldDir)) whichDir.push(ghostOldDir);
                }
                else
                {
                    if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: LEFT)) whichDir.push(3);
                    if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: RIGHT)) whichDir.push(1);
                    if(map -> can_change_direction(ghostTileX , ghostTileY , ghostOldDir)) whichDir.push(ghostOldDir);
                }
                int dir = rand() % (int) whichDir.size() + 1;
                while(dir > 1) whichDir.pop() , --dir;
                ghost -> set_ghost_direction(whichDir.top());
                while(!whichDir.empty()) whichDir.pop();
            }
            else
            {
                int dU = __INT32_MAX__ , dD = __INT32_MAX__ , dL = __INT32_MAX__ , dR = __INT32_MAX__;
                if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: UP))
                    dU = map -> get_distance(II(ghostTileX , ghostTileY - 1) , II(ghostNextTileX , ghostNextTileY) , Map :: UP);
                if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: DOWN))
                    dD = map -> get_distance(II(ghostTileX , ghostTileY + 1) , II(ghostNextTileX , ghostNextTileY) , Map :: DOWN);
                if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: LEFT))
                    dL = map -> get_distance(II(ghostTileX - 1 , ghostTileY) , II(ghostNextTileX , ghostNextTileY) , Map :: LEFT);
                if(map -> can_change_direction(ghostTileX , ghostTileY , Map :: RIGHT))
                    dR = map -> get_distance(II(ghostTileX + 1 , ghostTileY) , II(ghostNextTileX , ghostNextTileY) , Map :: RIGHT);

                if(ghostOldDir == Map :: UP)
                {
                    int dMin = min({dU , dL , dR});
                    if(dMin == dU) ghost -> set_ghost_direction(Map :: UP);
                    else if(dMin == dL) ghost -> set_ghost_direction(Map :: LEFT);
                    else ghost -> set_ghost_direction(Map :: RIGHT);
                }
                else if(ghostOldDir == Map :: DOWN)
                {
                    int dMin = min({dD , dL , dR});
                    if(dMin == dD) ghost -> set_ghost_direction(Map :: DOWN);
                    else if(dMin == dL) ghost -> set_ghost_direction(Map :: LEFT);
                    else ghost -> set_ghost_direction(Map :: RIGHT);
                }
                else if(ghostOldDir == Map :: LEFT)
                {
                    int dMin = min({dU , dD , dL});
                    if(dMin == dU) ghost -> set_ghost_direction(Map :: UP);
                    else if(dMin == dD) ghost -> set_ghost_direction(Map :: DOWN);
                    else ghost -> set_ghost_direction(Map :: LEFT);
                }
                else if(ghostOldDir == Map :: RIGHT)
                {
                    int dMin = min({dU , dD , dR});
                    if(dMin == dU) ghost -> set_ghost_direction(Map :: UP);
                    else if(dMin == dR) ghost -> set_ghost_direction(Map :: RIGHT);
                    else ghost -> set_ghost_direction(Map :: DOWN);
                }
            }
        }
        if(map -> can_change_direction(ghostTileX , ghostTileY , ghost -> get_ghost_direction())) ghost -> ghost_moving();
    }
    else
    {
        if(map -> can_change_direction(ghostTileX , ghostTileY , ghost -> get_ghost_direction())) ghost -> ghost_moving();
        else
        {
            if(ghostTileX * TILE_SIZE == ghostPosX && ghostTileY * TILE_SIZE != ghostPosY && ghost -> get_ghost_direction() % 2 == 0) ghost -> ghost_moving();
            else if(ghostTileY * TILE_SIZE == ghostPosY && ghostTileX * TILE_SIZE != ghostPosX && ghost -> get_ghost_direction() % 2 == 1) ghost -> ghost_moving();
        }
    }
    ghost -> go_through_portal();
    if(ghostPosX == ghostNextTileX * TILE_SIZE && ghostPosY == ghostNextTileY * TILE_SIZE)
    {
        if(ghost -> is_dead())
        {
            ghost -> set_dead(false);
            sound_manager -> insert_playlist(SoundManager :: REVIVAL_GHOST);
        }
        else if(ghost == greendy)
        {
            tick_manager -> greendy_start_chase_pacman();
            ghost -> reset_object_tile(ghostTileX , ghostTileY);
            apple -> destroy_item();
            eat_green_apple = true;
        }
    }
    pacman_meat_ghost(ghost);
}

void Engine :: loop_game(bool &exit_to_menu)
{
    if(game_manager -> eat_all_coins())
    {
        if(wait_time > 0) --wait_time;
        else
        {
            game_manager -> next_level();
            tick_manager -> reset_tick(game_manager -> get_level());
            respawn_object();
            map -> map_reset();
        }
        return;
    }
    if(Mix_Playing(2) || Mix_Playing(4))
    {
        if(Mix_Playing(2)) tick_manager -> pause_tick(true);
        return;
    }
    if(pacman -> is_dead())
    {
        int decision = game_manager -> get_player_decision();
        if(running_endgame_board)
        {
            if(decision == GameManager :: AGAIN) new_game();
            else if(decision == GameManager :: QUIT) exit_to_menu = true;
        }
        return;
    }

    tick_manager -> update_status();

    int pacman_tile_col = pacman -> get_tile_col();
    int pacman_tile_row = pacman -> get_tile_row();
    int pacman_screen_pos_col = pacman -> get_screen_pos_col();
    int pacman_screen_pos_row = pacman -> get_screen_pos_row();
    int last_direction = -1;
    if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();

    if(!pacman -> is_dead() && last_direction != -1)
    {
        if(pacman_tile_col * TILE_SIZE == pacman_screen_pos_col && pacman_tile_row * TILE_SIZE == pacman_screen_pos_row)
        {
            if(map -> is_tile_cross(pacman_tile_col , pacman_tile_row))
            {
                if(!pacman -> is_special_empty() && pacman -> get_pacman_special_cross_id() == II(pacman_tile_col , pacman_tile_row)) pacman -> pacman_turn();
            }
            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , pacman -> get_pacman_direction())) pacman -> pacman_moving();
            else pacman -> pacman_stopmoving();
        }
        else
        {
            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , last_direction)) pacman -> pacman_moving();
            else
            {
                if(pacman_tile_col * TILE_SIZE == pacman_screen_pos_col && pacman_tile_row * TILE_SIZE != pacman_screen_pos_row) pacman -> pacman_moving();
                else if(pacman_tile_col * TILE_SIZE != pacman_screen_pos_col && pacman_tile_row * TILE_SIZE == pacman_screen_pos_row) pacman -> pacman_moving();
                else pacman -> pacman_stopmoving();
            }
        }
    }

    int remainCoin = game_manager -> get_remain_coins();
    if(remainCoin < 50) sound_manager -> insert_playlist(SoundManager :: MOVE_3);
    else if(remainCoin < 100) sound_manager -> insert_playlist(SoundManager :: MOVE_2);
    else if(remainCoin < 150) sound_manager -> insert_playlist(SoundManager :: MOVE_1);
    else sound_manager -> insert_playlist(SoundManager :: MOVE_0);

    pacman_tile_col = pacman -> get_tile_col();
    pacman_tile_row = pacman -> get_tile_row();
    int typeOfCoin = map -> pacman_eat_coins(pacman_tile_col , pacman_tile_row);
    if(typeOfCoin != GameManager :: NOT_COIN)
    {
        game_manager -> eat_coin(typeOfCoin);
        sound_manager -> insert_playlist(SoundManager :: EAT_DOT);
        if(typeOfCoin == GameManager :: SUPER_COIN)
        {
            tick_manager -> set_frighten_time();
            sound_manager -> insert_playlist(SoundManager :: GHOST_TURN_FRIGHTEN);
            if(!blinky -> is_dead()) blinky -> set_ghost_frighten(true);
            if(!pinky  -> is_dead()) pinky  -> set_ghost_frighten(true);
            if(!inky   -> is_dead()) inky   -> set_ghost_frighten(true);
            if(!clyde  -> is_dead()) clyde  -> set_ghost_frighten(true);
            if(greendy != nullptr && !greendy -> is_dead()) greendy -> set_ghost_frighten(true);
            if(friendy != nullptr) tick_manager -> friendy_start_chase_pacman();
        }
    }

    if(!tick_manager -> is_frighten_time())
    {
        sound_manager -> insert_playlist(SoundManager :: NORMAL_GHOST);
        blinky -> set_ghost_frighten(false);
        pinky  -> set_ghost_frighten(false);
        inky   -> set_ghost_frighten(false);
        clyde  -> set_ghost_frighten(false);
        if(greendy != nullptr) greendy -> set_ghost_frighten(false);
    }

    bool scatter = tick_manager -> is_scatter_time();
    blinky -> set_ghost_scatter(scatter);
    pinky  -> set_ghost_scatter(scatter);
    inky   -> set_ghost_scatter(scatter);
    clyde  -> set_ghost_scatter(scatter);

    if(!tick_manager -> is_greeny_chase_time())
    {
        eat_green_apple = false;
    }
    else if(apple -> is_destroyed())
    {
        int r = rand() % 4;
        if(r == 0) apple -> spawn_at(1 , 1);
        else if(r == 1) apple -> spawn_at(26 , 1);
        else if(r == 2) apple -> spawn_at(26 , 29);
        else apple -> spawn_at(1 , 29);
    }

    pacman_screen_pos_col = pacman -> get_screen_pos_col();
    pacman_screen_pos_row = pacman -> get_screen_pos_row();
    last_direction = -1;
    if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();

    if(!pacman -> is_dead())
    {
        tick_manager -> pause_tick(false);
        if(blinky -> is_dead()) blinky -> set_ghost_destination(13 , 11);
        else if(!blinky -> is_ghost_scatter()) blinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row);
        else blinky -> set_ghost_destination(Ghost :: DEFAULT_BLINKY_TILE_COL , Ghost :: DEFAULT_BLINKY_TILE_ROW);

        if(pinky -> is_dead()) pinky -> set_ghost_destination(13 , 11);
        else if(!pinky -> is_ghost_scatter())
        {
            if(last_direction == Map :: UP) pinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row - 4);
            else if(last_direction == Map :: DOWN) pinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row + 4);
            else if(last_direction == Map :: LEFT) pinky -> set_ghost_destination(pacman_tile_col - 4 , pacman_tile_row);
            else if(last_direction == Map :: RIGHT) pinky -> set_ghost_destination(pacman_tile_col + 4 , pacman_tile_row);
        }
        else pinky -> set_ghost_destination(Ghost :: DEFAULT_PINKY_TILE_COL , Ghost :: DEFAULT_PINKY_TILE_ROW);

        if(inky -> is_dead()) inky -> set_ghost_destination(13 , 11);
        else if(!inky -> is_ghost_scatter()) inky -> set_ghost_destination(2 * pacman_tile_col - blinky -> get_tile_col() , 2 * pacman_tile_row - blinky -> get_tile_row());
        else inky -> set_ghost_destination(Ghost :: DEFAULT_INKY_TILE_COL , Ghost :: DEFAULT_INKY_TILE_ROW);

        if(clyde -> is_dead()) clyde -> set_ghost_destination(13 , 11);
        else if(!clyde -> is_ghost_scatter())
        {
            int dx = pacman_tile_col - clyde -> get_tile_col();
            int dy = pacman_tile_row - clyde -> get_tile_row();
            if(dx * dx + dy * dy <= 64) clyde -> set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);
            else clyde -> set_ghost_destination(pacman_tile_col , pacman_tile_row);
        }
        else clyde -> set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);

        if(greendy != nullptr)
        {
            if(greendy -> is_dead()) greendy -> set_ghost_destination(13 , 11);
            else if(!eat_green_apple) greendy -> set_ghost_destination(apple -> get_tile_col() , apple -> get_tile_row());
            else if(!greendy -> is_ghost_frighten()) greendy -> set_ghost_destination(pacman_tile_col , pacman_tile_row , 2);
        }

        if(friendy != nullptr && tick_manager -> is_friendy_chase_time())
        {
            friendy -> set_ghost_destination(pacman_tile_col , pacman_tile_row , 1);
        }
    }

    pacman -> go_through_portal();
    ghost_move(blinky);
    ghost_move(pinky);
    ghost_move(inky);
    ghost_move(clyde);
    ghost_move(greendy);
    ghost_move(friendy);

    game_manager -> handle_ghost_pos(pinky , inky , clyde , greendy);

    if(game_manager -> eat_all_coins())
    {
        sound_manager -> insert_playlist(SoundManager :: WINNING);
        wait_time = 100;
    }
}
