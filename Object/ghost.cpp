#include "ghost.h"
#include <random>
#include <iostream>

// Hàm khởi tạo Ghost với vị trí bắt đầu và trạng thái trong lồng
Ghost :: Ghost(int start_tile_col , int start_tile_row , bool _in_cage) : Object(start_tile_col , start_tile_row)
{
    frighten_mode = 0;                        // Trạng thái ghost có đang sợ hay không (0: bình thường)
    accele = 1;                               // Tăng tốc tạm thời (dùng khi ghost ăn táo hoặc hồi sinh)
    ghost_speed = 2;                          // Tốc độ mặc định của ghost
    scatter_mode = false;                     // Trạng thái ghost có đang ở chế độ "thăm dò" không
    target_tile_col = start_tile_col;         // Vị trí mục tiêu
    target_tile_row = start_tile_row;
    in_cage = _in_cage;                       // Trạng thái ghost có đang trong lồng không
    ghost_direction = (in_cage ? UP : RIGHT); // Nếu trong lồng thì hướng lên, còn không thì đi sang phải
}

// Trả về tọa độ cột của tile mục tiêu mà ghost hướng tới
int Ghost :: get_target_tile_col() const
{
    return target_tile_col;
}

// Trả về tọa độ hàng của tile mục tiêu
int Ghost :: get_target_tile_row() const
{
    return target_tile_row;
}

// Trả về hướng hiện tại của ghost (UP/DOWN/LEFT/RIGHT)
int Ghost :: get_ghost_direction() const
{
    return ghost_direction;
}

// Cập nhật hướng di chuyển của ghost
void Ghost :: set_ghost_direction(int direction)
{
    ghost_direction = direction;
}

// Cập nhật trạng thái sợ hãi của ghost
void Ghost :: set_ghost_frighten(const bool status)
{
    if(is_ghost_in_cage()) return; // Nếu còn trong lồng thì không chuyển trạng thái sợ

    if(frighten_mode != status)
        update_screen_pos(); // Cập nhật lại vị trí để đảm bảo đồng bộ

    frighten_mode = status;

    if(status) {
        // Nếu chuyển sang sợ hãi → quay đầu 180 độ
        ghost_direction = (ghost_direction + 2) % 4;
        accele = 1; // Dừng tăng tốc (trở lại bình thường)
    }
}

// Cập nhật trạng thái ghost có đang ở chế độ scatter (đi lang thang) không
void Ghost :: set_ghost_scatter(const bool status)
{
    scatter_mode = status;
}

// Trả về true nếu ghost đang scatter
bool Ghost :: is_ghost_scatter() const
{
    return scatter_mode;
}

// Trả về true nếu ghost đang trong trạng thái sợ hãi (đã ăn power dot)
bool Ghost :: is_ghost_frighten() const
{
    return frighten_mode;
}

// Đặt mục tiêu mới cho ghost (vị trí cần đuổi theo)
void Ghost :: set_ghost_destination(int tile_col , int tile_row , int _accele)
{
    accele = _accele; // Hệ số tốc độ
    target_tile_col = tile_col;
    target_tile_row = tile_row;
}

// Hàm xử lý di chuyển cho ghost
void Ghost :: ghost_moving()
{
    // Xác định tốc độ hiện tại dựa trên trạng thái
    if(accele == 1) {
        if(is_dead()) ghost_speed = 4;             // Nếu là hồn → đi nhanh để về lồng
        else if(is_ghost_frighten()) ghost_speed = 1; // Nếu sợ hãi → đi chậm
        else ghost_speed = 2;                      // Bình thường
    }
    else ghost_speed = 4; // Nếu accele được set bên ngoài → tăng tốc

    int speed_col = 0;
    int speed_row = 0;
    int dir = -1;

    // Tính vận tốc dựa theo hướng đi hiện tại
    if(ghost_direction == UP) {
        speed_row = -ghost_speed;
        dir = UP;
    }
    if(ghost_direction == DOWN) {
        speed_row = ghost_speed;
        dir = DOWN;
    }
    if(ghost_direction == RIGHT) {
        speed_col = ghost_speed;
        dir = RIGHT;
    }
    if(ghost_direction == LEFT) {
        speed_col = -ghost_speed;
        dir = LEFT;
    }

    // Cập nhật tốc độ và hướng
    change_speed_and_direction(speed_col , speed_row , dir);

    // Gọi hàm move() kế thừa từ Object để cập nhật vị trí
    move();
}

// Hồi sinh lại ghost ở vị trí cụ thể và xác định có trong lồng hay không
void Ghost :: ghost_respawn(const int tile_col, const int tile_row, const bool _in_cage)
{
    reset_object_tile(tile_col , tile_row); // Đặt lại vị trí
    in_cage = _in_cage;

    // Nếu không trong lồng thì chọn ngẫu nhiên LEFT/RIGHT, còn nếu trong lồng thì hướng là UP
    if(!in_cage) {
        ghost_direction = (rand() % 2 == 0) ? LEFT : RIGHT;
    }
    else ghost_direction = UP;
}

// Trả về trạng thái ghost có đang bị giữ trong lồng không
bool Ghost :: is_ghost_in_cage() const
{
    return in_cage;
}
