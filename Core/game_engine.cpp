#include "game_engine.h"
#include <limits>
#include <random>
#include <time.h>

using namespace std;

typedef std :: pair <int , int> II;
typedef std :: pair <int , std :: pair <int , int> > IP;

// Khởi tạo tất cả thành phần chính của game
void Engine :: init_game(SDL_Renderer* &renderer)
{
    map = new Map(); // bản đồ game

    object_texture = new Texture_Source(); // load texture cho đối tượng
    object_texture -> load_tile_texture(renderer);
    object_texture -> load_pacman_and_ghost_texture(renderer);

    tick_manager = new TickManager(); // quản lý thời gian của ghost
    game_manager = new GameManager(renderer); // quản lý Point, Life, Level
    sound_manager = new SoundManager(); // Quản lý âm thanh
    sound_manager -> load_sound();

    srand(time(nullptr)); // phục vụ cho hàm rand
    new_game(); // Bắt đầu game mới

    // Load ảnh thông báo "nect level"
    SDL_Surface* image = IMG_Load("Assets/Menu Image/nextlevel.png");
    next_level = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);

    // Load ảnh thông báo "ready"
    image = IMG_Load("Assets/Menu Image/ready.png");
    ready = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
}

// Reset game, tạo lại pacman, ghost và bản đồ
void Engine :: new_game()
{
    map -> map_reset();
    game_manager -> reset_game();

    // xóa và tạo mới pacman
    delete pacman;
    pacman = new Pacman();

    // xóa và tạo mới ghost
    delete blinky;
    blinky = new Ghost(12 , 11 , false);



    delete pinky;
    pinky = new Ghost(13 , 14 , true);


    delete inky;
    inky = new Ghost(11 , 14 , true);


    delete clyde;
    clyde = new Ghost(15 , 14 , true);


    sound_manager -> insert_playlist(SoundManager :: START); // ơhát nhạc bắt đầu
    tick_manager -> reset_tick(game_manager -> get_level()); // reset thời gian của ghost
    tick_manager -> pause_tick(true); // tạm dừng ghost di chuyển
    running_endgame_board = false; // xóa bảng kết thúc
}


void Engine :: respawn_object()
{
    // xóa và tạo lại pacman
    delete pacman;
    pacman = new Pacman();

    // xóa và tạo lại các ghost
    delete blinky;
    blinky = new Ghost(13 , 11 , false);

    delete pinky;
    pinky = new Ghost(13 , 14 , true);

    delete inky;
    inky = new Ghost(11 , 14 , true);

    delete clyde;
    clyde = new Ghost(15 , 14 , true);

    sound_manager -> reset_sound(); // reset lại âm thanh
    tick_manager -> pause_tick(false); // cho phép ghost tiếp tục di chuyển
}

void Engine :: handle_event_engine(SDL_Event &event , std :: vector <std :: string> &score_data)
{
    if(Mix_Playing(2) || Mix_Playing(4)) return; // nếu đang phát start/dead/eat_ghost thì return
    if(pacman -> is_dead()) // nếu pacman bị ghost giết
    {
        if(running_endgame_board) { // nếu bảng kết thúc đang chạy
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

            // vị trí trên tile hiện tại của pacman
            int pacman_tile_col = pacman -> get_tile_col();
            int pacman_tile_row = pacman -> get_tile_row();

            // vị trí trên màn hình hiện tại của pacman
            int pacman_screen_pos_col = pacman -> get_screen_pos_col();
            int pacman_screen_pos_row = pacman -> get_screen_pos_row();

            // gán hướng đi hiện tại
            if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();

            // gán hướng người chơi vừa nhân trên bàn phím
            if(event.key.keysym.sym == SDLK_UP    || event.key.keysym.sym == SDLK_w) new_direction = Map :: UP;
            if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) new_direction = Map :: RIGHT;
            if(event.key.keysym.sym == SDLK_DOWN  || event.key.keysym.sym == SDLK_s) new_direction = Map :: DOWN;
            if(event.key.keysym.sym == SDLK_LEFT  || event.key.keysym.sym == SDLK_a) new_direction = Map :: LEFT;

            if(last_direction == -1) // nếu pacman đang đứng yên
            {
                if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction)) // nếu đổi hướng được thì hướng hiện tại sẽ là hướng mới đó
                {
                    pacman -> clear_special();
                    pacman -> push_to_direction(new_direction);
                }
            }
            else
            {
                if(new_direction % 2 == last_direction % 2) // trường hợp trùng với hướng cũ hoặc ngược lại (quay đầu)
                {
                    if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction)) // nếu đổi hướng được thì hướng hiện tại sẽ là hướng mới đó
                    {
                        pacman -> clear_special();
                        pacman -> push_to_direction(new_direction);
                    }
                }
                else
                {
                    std :: pair <int , int> next_cross_id = map -> get_nearest_cross_tile_id(pacman_tile_col , pacman_tile_row , last_direction);
                    if(last_direction % 2 == 1 && new_direction % 2 == 0) // hướng đi hiện tại là left/right và hướng mới là up/down
                    {
                        if(pacman_screen_pos_row == pacman_tile_row * TILE_SIZE) // nếu pacman ở đúng vị trí ngã rẽ
                        {
                            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction)) {
                                pacman -> push_to_special(new_direction , II(pacman_tile_col , pacman_tile_row));
                            }
                            // nếu ngã rẽ tồn tại và hướng mới sẽ không va vào tường trong trường hợp là ngã 2, ngã 3 và độ dự đoán phải nhỏ hơn hoặc bằng 2 blocks
                            else if(next_cross_id != II(-1 , -1) && !map -> is_beside_cross_is_wall(next_cross_id , new_direction) && abs(pacman_screen_pos_col - next_cross_id.first * TILE_SIZE) <= 2 * TILE_SIZE) {
                                pacman -> push_to_special(new_direction , next_cross_id);
                            }
                        }
                    }
                    else if(last_direction % 2 == 0 && new_direction % 2 == 1) // hướng đi hiện tại là up/down và hướng mới là left/right
                    {
                        if(pacman_screen_pos_col == pacman_tile_col * TILE_SIZE) // nếu pacman ở đúng vị trí ngã rẽ
                        {
                            if(map -> can_change_direction(pacman_tile_col , pacman_tile_row , new_direction)) {
                                pacman -> push_to_special(new_direction , II(pacman_tile_col , pacman_tile_row));
                            }
                            else if(next_cross_id != II(-1 , -1) && !map -> is_beside_cross_is_wall(next_cross_id , new_direction) && abs(pacman_screen_pos_row - next_cross_id.second * TILE_SIZE) <= 2 * TILE_SIZE) {
                                pacman -> push_to_special(new_direction , next_cross_id);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Engine :: render_game(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data)
{
    tick_manager -> stablize_FPS(); // cân bằng tốc độ khung hình để tránh game chạy quá nhanh
    SDL_Rect display_rect; // vùng chữ nhật sẽ in lên trên màn hình

    // vẽ lại toàn bộ map
    for(int col = 0; col < map -> get_map_width(); col++)
    {
        for(int row = 0; row < map -> get_map_height(); row++)
        {
            display_rect = {col * TILE_SIZE + 217 , row * TILE_SIZE , TILE_SIZE , TILE_SIZE};
            object_texture -> render_tile_texture(renderer , map -> get_tile_id(col , row) , &display_rect);
        }
    }

    if(!running_endgame_board) // nếu bảng kết thúc đang hiển thị
    {
        int current_direction = -1; // hướng đi hiện tại của pacman
        if(!pacman -> is_direction_empty()) { // gán hướng đi hiện tại nếu có
            current_direction = pacman -> get_pacman_direction();
        }
        if(!pacman -> is_dead()) // nếu pacman chưa chết
        {
            // vẽ tất cả các ghost
            render_ghost(renderer , blinky , Texture_Source :: BLINKY);
            render_ghost(renderer , pinky , Texture_Source :: PINKY);
            render_ghost(renderer , inky , Texture_Source :: INKY);
            render_ghost(renderer , clyde , Texture_Source :: CLYDE);

            // Hiển thị chữ ready nếu dang phát kênh 2
            if(Mix_Playing(2))
            {
                display_rect = {441 - 82 , 285 - 15 - 7 , 164 , 30};
                SDL_RenderCopy(renderer , ready , nullptr , &display_rect);
            }
        }
        if(pacman -> is_dead()) // nếu pacman chết
        {
            if(object_texture -> is_pacman_dead()) // nếu đã kết thúc hoạt ảnh chết của pacman
            {
                if(game_manager -> get_remain_life() > 0) respawn_object(); // nếu còn mạng thì respawn
                else // nếu không còn mạng nào thì hiển thị bảng kết thúc và kiểm tra điểm số
                {
                    running_endgame_board = true;
                    game_manager -> check_score_data(score_data);
                }
            }
            // nếu hoạt ảnh chưa kết thúc thì tiếp tục render
            else object_texture -> render_pacman_texture(renderer , pacman -> get_screen_pos_col() , pacman -> get_screen_pos_row() , Texture_Source :: DEAD_PACMAN);
        }
        // nếu còn sống thì render theo hướng di chuyển như bình thường
        else object_texture -> render_pacman_texture(renderer , pacman -> get_screen_pos_col() , pacman -> get_screen_pos_row() , current_direction);
        if(wait_time > 0) // nếu đang ở trạng thái chuyển sang màn mới thì hiện chữ next level
        {
            display_rect = {441 - 97 , 248 - 52 , 194 , 104};
            SDL_RenderCopy(renderer , next_level , nullptr , &display_rect);
        }

        // nếu pacman đang ăn ghost thì render điểm có được từ việc ăn
        if(Mix_Playing(4))
        {
            object_texture -> render_ghost_score(renderer , game_manager -> get_eaten_ghost_pos_col() , game_manager -> get_eaten_ghost_pos_row() , game_manager -> get_eaten_ghost_streak());
        }
        sound_manager -> play_sound(); // phát các âm thanh tương ứng với trạng thái hiện tại của game
    }
    // nếu bảng kết thúc đang hiển thị thì render lên màn hình, còn không thì hiển thị mấy thông số điểm, mạng, level
    if(running_endgame_board) game_manager -> run_endgame_board(renderer);
    else game_manager -> render_HUD(renderer);
}

void Engine :: pacman_meat_ghost(Ghost* &ghost)
{
    if(ghost -> is_dead()) return; // nếu gặp phải hồn ma thì thôi

    // tính khoảng cách euclid giữa pacman và ghost
    int distance = (pacman -> get_screen_pos_col() - ghost -> get_screen_pos_col()) * (pacman -> get_screen_pos_col() - ghost -> get_screen_pos_col()) +
                   (pacman -> get_screen_pos_row() - ghost -> get_screen_pos_row()) * (pacman -> get_screen_pos_row() - ghost -> get_screen_pos_row());

    if(distance <= 9) // nếu khoảng cách đủ nhỏ để tính là va chạm
    {
        if(ghost -> is_ghost_frighten()) // nếu ghost đang trong chế độ sợ hãi
        {
            game_manager -> eat_ghost(ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row());
            ghost -> set_dead(true);
            ghost -> set_ghost_frighten(false);
            sound_manager -> insert_playlist(SoundManager :: EAT_GHOST);
            sound_manager -> insert_playlist(SoundManager :: GHOST_GO_HOME);
        }
        else // nếu không, pacman chết
        {
            pacman -> set_dead(true , 1);
            game_manager  -> lost_a_life();
            sound_manager -> insert_playlist(SoundManager :: DEAD);
            tick_manager  -> pause_tick(true); // tạm dừng toàn bộ di chuyển của ghost
        }
    }
}

void Engine :: render_ghost(SDL_Renderer* &renderer , Ghost* &ghost , int ghost_id)
{
    if(ghost == nullptr) return; // nếu không có ghost nào để vẽ thì thôi
    if(ghost -> is_dead()) // nếu ghost bị ăn thì render ra hồn
    {
        object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , Texture_Source :: GHOST_SPIRIT , ghost -> get_ghost_direction());
    }
    else if(ghost -> is_ghost_frighten()) // nếu ghost đang sợ hãi mode
    {
        if(tick_manager -> remain_frighten_time() < 2.0) // nếu gần hết thời gian thì nhấp nháy
        {
            object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , Texture_Source :: FRIGHTEN_GHOST_2);
        }
        else
        {
            object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , Texture_Source :: FRIGHTEN_GHOST_1);
        }
    }
    else // nếu ghost bình thường
    {
        object_texture -> render_ghost_texture(renderer , ghost -> get_screen_pos_col() , ghost -> get_screen_pos_row() , ghost_id , ghost -> get_ghost_direction());
    }
}

void Engine :: ghost_move(Ghost* &ghost)
{
    if(ghost == nullptr) return; // nếu không tồn tại ghost nào thì thôi

    // thông tin về vị trí và hương đi hiện tại của ghost
    int ghost_tile_col = ghost -> get_tile_col();
    int ghost_tile_row = ghost -> get_tile_row();
    int ghost_screen_pos_col = ghost -> get_screen_pos_col();
    int ghost_screen_pos_row = ghost -> get_screen_pos_row();
    int ghost_old_direction = ghost -> get_ghost_direction();
    int ghost_target_tile_col = ghost -> get_target_tile_col();
    int ghost_target_tile_row = ghost -> get_target_tile_row();

    // nếu ghost đang đứng giữa cái ô
    if(ghost_tile_col * TILE_SIZE == ghost_screen_pos_col && ghost_tile_row * TILE_SIZE == ghost_screen_pos_row)
    {
        if(map -> is_tile_cross(ghost_tile_col , ghost_tile_row)) // nếu cái ô đó là ngã rẽ
        {
            // nếu ghost đang sợ hãi
            if(ghost -> is_ghost_frighten())
            {
                // ghost chọn random hướng có thể đi (trừ hướng hiện tại)
                std :: stack <int> which_dir;
                if(ghost_old_direction % 2 == 1) // đang đi dọc (UP/DOWN)
                {
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: UP)) which_dir.push(Map :: UP);
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: DOWN)) which_dir.push(Map :: DOWN);
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , ghost_old_direction)) which_dir.push(ghost_old_direction);
                }
                else // đang đi ngang (RIGHT/LEFT)
                {
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: LEFT)) which_dir.push(Map :: LEFT);
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: RIGHT)) which_dir.push(Map :: RIGHT);
                    if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , ghost_old_direction)) which_dir.push(ghost_old_direction);
                }
                // chọn ngẫu nhiên một hướng ở trong stack
                int dir = rand() % (int) which_dir.size() + 1;
                while(dir > 1) which_dir.pop() , --dir;

                // đặt hướng di chuyển mới cho ghost
                ghost -> set_ghost_direction(which_dir.top());
                while(!which_dir.empty()) which_dir.pop();
            }
            else // nếu ghost đang trong trạng thái bình thường hoặc hồn ma
            {
                // tính khoảng cách ngắn nhất để ghost đến mục tiêu với 4 hướng
                int dist_up = INT_MAX ;
                int dist_down = INT_MAX ;
                int dist_left = INT_MAX ;
                int dist_right = INT_MAX;
                if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: UP)) {
                    dist_up = map -> get_distance(II(ghost_tile_col , ghost_tile_row - 1) , II(ghost_target_tile_col , ghost_target_tile_row) , Map :: UP);
                }
                if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: DOWN)) {
                    dist_down = map -> get_distance(II(ghost_tile_col , ghost_tile_row + 1) , II(ghost_target_tile_col , ghost_target_tile_row) , Map :: DOWN);
                }
                if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: LEFT)) {
                    dist_left = map -> get_distance(II(ghost_tile_col - 1 , ghost_tile_row) , II(ghost_target_tile_col , ghost_target_tile_row) , Map :: LEFT);
                }
                if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , Map :: RIGHT)) {
                    dist_right = map -> get_distance(II(ghost_tile_col + 1 , ghost_tile_row) , II(ghost_target_tile_col , ghost_target_tile_row) , Map :: RIGHT);
                }

                // Dựa vào hướng đi cũ mà ưu tiên
                if(ghost_old_direction == Map :: UP)
                {
                    int dist_min = min({dist_up , dist_left , dist_right});
                    if(dist_min == dist_up) ghost -> set_ghost_direction(Map :: UP);
                    else if(dist_min == dist_left) ghost -> set_ghost_direction(Map :: LEFT);
                    else ghost -> set_ghost_direction(Map :: RIGHT);
                }
                else if(ghost_old_direction == Map :: DOWN)
                {
                    int dist_min = min({dist_down , dist_left , dist_right});
                    if(dist_min == dist_down) ghost -> set_ghost_direction(Map :: DOWN);
                    else if(dist_min == dist_left) ghost -> set_ghost_direction(Map :: LEFT);
                    else ghost -> set_ghost_direction(Map :: RIGHT);
                }
                else if(ghost_old_direction == Map :: LEFT)
                {
                    int dist_min = min({dist_up , dist_down , dist_left});
                    if(dist_min == dist_up) ghost -> set_ghost_direction(Map :: UP);
                    else if(dist_min == dist_down) ghost -> set_ghost_direction(Map :: DOWN);
                    else ghost -> set_ghost_direction(Map :: LEFT);
                }
                else if(ghost_old_direction == Map :: RIGHT)
                {
                    int dist_min = min({dist_up , dist_down , dist_right});
                    if(dist_min == dist_up) ghost -> set_ghost_direction(Map :: UP);
                    else if(dist_min == dist_right) ghost -> set_ghost_direction(Map :: RIGHT);
                    else ghost -> set_ghost_direction(Map :: DOWN);
                }
            }
        }
        // nếu hướng đi mới hợp lệ -> thực hiện di chuyển thôi
        if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , ghost -> get_ghost_direction())) ghost -> ghost_moving();
    }
    else // ghost không ở giữa ô, nếu hướng hiện tại vẫn hợp lệ thì tiếp tục di chuyển
    {
        if(map -> can_change_direction(ghost_tile_col , ghost_tile_row , ghost -> get_ghost_direction())) ghost -> ghost_moving();
        else // nếu không hợp lệ
        {
            // nếu lệch trục mà đang đi dọc
            if(ghost_tile_col * TILE_SIZE == ghost_screen_pos_col && ghost_tile_row * TILE_SIZE != ghost_screen_pos_row && ghost -> get_ghost_direction() % 2 == 0) ghost -> ghost_moving();
            // nếu lệch trục mà đang đi ngang
            else if(ghost_tile_row * TILE_SIZE == ghost_screen_pos_row && ghost_tile_col * TILE_SIZE != ghost_screen_pos_col && ghost -> get_ghost_direction() % 2 == 1) ghost -> ghost_moving();
        }
    }

    ghost -> go_through_portal(); // kiểm tra xem ghost có qua cổng dịch chuyển không

    // nếu ghost đã đến ô mục tiêu
    if(ghost_screen_pos_col == ghost_target_tile_col * TILE_SIZE && ghost_screen_pos_row == ghost_target_tile_row * TILE_SIZE)
    {
        if(ghost -> is_dead()) // nếu ghost đã chết thì hồi sinh
        {
            ghost -> set_dead(false);
            sound_manager -> insert_playlist(SoundManager :: REVIVAL_GHOST);
        }
    }
    // kiểm tra va chạm giữa pacman và ghost
    pacman_meat_ghost(ghost);
}

void Engine :: loop_game(bool &exit_to_menu)
{
    if(game_manager -> eat_all_coins()) // nếu đã ăn hết coin thì chờ vài frame để qua màn mới
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
    // nếu pacman đã chết và bảng kết thúc hiển thị thì xử lý lựa chọn của người chơi
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

    tick_manager -> update_status(); // cập nhật trạng thái tick của ghost

    int pacman_tile_col = pacman -> get_tile_col();
    int pacman_tile_row = pacman -> get_tile_row();
    int pacman_screen_pos_col = pacman -> get_screen_pos_col();
    int pacman_screen_pos_row = pacman -> get_screen_pos_row();
    int last_direction = -1;

    if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();

    // di chuyển nếu pacman còn sống
    if(!pacman -> is_dead() && last_direction != -1)
    {
        if(pacman_tile_col * TILE_SIZE == pacman_screen_pos_col && pacman_tile_row * TILE_SIZE == pacman_screen_pos_row)
        {
            // nếu pacman ở đúng ô ngã rẽ và có hướng đi đặc biệt hợp lệ -> thực hiện turn
            if(map -> is_tile_cross(pacman_tile_col , pacman_tile_row))
            {
                if(!pacman -> is_special_empty() && pacman -> get_pacman_special_cross_id() == II(pacman_tile_col , pacman_tile_row)) pacman -> pacman_turn();
            }
            // nếu hướng đi hợp lệ thig tiếp tục, còn không thì dừng
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

      // Điều chỉnh âm thanh di chuyển theo số lượng coin còn lại
    int remainCoin = game_manager -> get_remain_coins();
    if(remainCoin < 50) sound_manager -> insert_playlist(SoundManager :: MOVE_3);
    else if(remainCoin < 100) sound_manager -> insert_playlist(SoundManager :: MOVE_2);
    else if(remainCoin < 150) sound_manager -> insert_playlist(SoundManager :: MOVE_1);
    else sound_manager -> insert_playlist(SoundManager :: MOVE_0);

    // Kiểm tra ăn coin
    pacman_tile_col = pacman -> get_tile_col();
    pacman_tile_row = pacman -> get_tile_row();
    int typeOfCoin = map -> pacman_eat_coins(pacman_tile_col , pacman_tile_row);
    if(typeOfCoin != GameManager :: NOT_COIN)
    {
        game_manager -> eat_coin(typeOfCoin);
        sound_manager -> insert_playlist(SoundManager :: EAT_DOT);
        if(typeOfCoin == GameManager :: SUPER_COIN)
        {
            // Kích hoạt trạng thái frighten cho tất cả ghost còn sống
            tick_manager -> set_frighten_time();
            sound_manager -> insert_playlist(SoundManager :: GHOST_TURN_FRIGHTEN);
            if(!blinky -> is_dead()) blinky -> set_ghost_frighten(true);
            if(!pinky  -> is_dead()) pinky  -> set_ghost_frighten(true);
            if(!inky   -> is_dead()) inky   -> set_ghost_frighten(true);
            if(!clyde  -> is_dead()) clyde  -> set_ghost_frighten(true);
        }
    }

    // Nếu hết thời gian frighten → trả ghost về trạng thái bình thường
    if(!tick_manager -> is_frighten_time())
    {
        sound_manager -> insert_playlist(SoundManager :: NORMAL_GHOST);
        blinky -> set_ghost_frighten(false);
        pinky  -> set_ghost_frighten(false);
        inky   -> set_ghost_frighten(false);
        clyde  -> set_ghost_frighten(false);
    }

    // Cập nhật trạng thái scatter cho các ghost chính
    bool scatter = tick_manager -> is_scatter_time();
    blinky -> set_ghost_scatter(scatter);
    pinky  -> set_ghost_scatter(scatter);
    inky   -> set_ghost_scatter(scatter);
    clyde  -> set_ghost_scatter(scatter);

    // Cập nhật hướng di chuyển cho từng ghost dựa theo trạng thái và AI riêng
    pacman_screen_pos_col = pacman -> get_screen_pos_col();
    pacman_screen_pos_row = pacman -> get_screen_pos_row();
    last_direction = -1;
    if(!pacman -> is_direction_empty()) last_direction = pacman -> get_pacman_direction();

    if(!pacman -> is_dead())
    {
        tick_manager -> pause_tick(false); // mở lại tick nếu Pacman sống

        // Ghost Blinky: luôn đuổi Pacman
        if(blinky -> is_dead()) blinky -> set_ghost_destination(13 , 11);
        else if(!blinky -> is_ghost_scatter()) blinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row);
        else blinky -> set_ghost_destination(Ghost :: DEFAULT_BLINKY_TILE_COL , Ghost :: DEFAULT_BLINKY_TILE_ROW);

        // Pinky: đuổi theo vị trí trước mặt Pacman (4 ô)
        if(pinky -> is_dead()) pinky -> set_ghost_destination(13 , 11);
        else if(!pinky -> is_ghost_scatter())
        {
            if(last_direction == Map :: UP) pinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row - 4);
            else if(last_direction == Map :: DOWN) pinky -> set_ghost_destination(pacman_tile_col , pacman_tile_row + 4);
            else if(last_direction == Map :: LEFT) pinky -> set_ghost_destination(pacman_tile_col - 4 , pacman_tile_row);
            else if(last_direction == Map :: RIGHT) pinky -> set_ghost_destination(pacman_tile_col + 4 , pacman_tile_row);
        }
        else pinky -> set_ghost_destination(Ghost :: DEFAULT_PINKY_TILE_COL , Ghost :: DEFAULT_PINKY_TILE_ROW);

        // Inky: AI dựa vào vị trí Pacman và Blinky
        if(inky -> is_dead()) inky -> set_ghost_destination(13 , 11);
        else if(!inky -> is_ghost_scatter()) inky -> set_ghost_destination(2 * pacman_tile_col - blinky -> get_tile_col() , 2 * pacman_tile_row - blinky -> get_tile_row());
        else inky -> set_ghost_destination(Ghost :: DEFAULT_INKY_TILE_COL , Ghost :: DEFAULT_INKY_TILE_ROW);

        // Clyde: nếu gần Pacman thì chạy ngược, nếu xa thì đuổi theo
        if(clyde -> is_dead()) clyde -> set_ghost_destination(13 , 11);
        else if(!clyde -> is_ghost_scatter())
        {
            int dx = pacman_tile_col - clyde -> get_tile_col();
            int dy = pacman_tile_row - clyde -> get_tile_row();
            if(dx * dx + dy * dy <= 64) clyde -> set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);
            else clyde -> set_ghost_destination(pacman_tile_col , pacman_tile_row);
        }
        else clyde -> set_ghost_destination(Ghost :: DEFAULT_CLYDE_TILE_COL , Ghost :: DEFAULT_CLYDE_TILE_ROW);
    }

    // Di chuyển Pacman qua cổng nếu cần
    pacman -> go_through_portal();

    // Di chuyển tất cả ghost
    ghost_move(blinky);
    ghost_move(pinky);
    ghost_move(inky);
    ghost_move(clyde);

    // Kiểm tra điều kiện mở lồng ghost
    game_manager -> handle_ghost_pos(pinky , inky , clyde);

    // Nếu ăn hết coin thì chuẩn bị qua màn
    if(game_manager -> eat_all_coins())
    {
        sound_manager -> insert_playlist(SoundManager :: WINNING);
        wait_time = 100;
    }
}
