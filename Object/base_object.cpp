#include "base_object.h"
#include <iostream>
#include <cmath>

// Hàm khởi tạo đối tượng cơ bản với vị trí tile (ô) và tốc độ di chuyển
Object :: Object(int _tile_col , int _tile_row , int _speed_col , int _speed_row)
{
    tile_col = _tile_col;
    tile_row = _tile_row;

    // Chỉ gán tốc độ nếu khác 0
    if(_speed_col != 0) speed_col = _speed_col;
    if(_speed_row != 0) speed_row = _speed_row;

    // Tính toán vị trí trên màn hình dựa vào tile
    screen_pos_col = tile_col * TILE_SIZE + TILE_SIZE / 2;
    screen_pos_row = tile_row * TILE_SIZE;

    dead = false; // Mặc định đối tượng còn sống
}

// Cập nhật vị trí màn hình của đối tượng khi di chuyển
void Object :: move()
{
    screen_pos_col += speed_col;
    screen_pos_row += speed_row;

    // Cập nhật lại vị trí tile tương ứng
    if(screen_pos_col % TILE_SIZE == 0) tile_col = screen_pos_col / TILE_SIZE;
    else tile_col = (screen_pos_col + TILE_SIZE / 2) / TILE_SIZE;

    if(screen_pos_row % TILE_SIZE == 0) tile_row = screen_pos_row / TILE_SIZE;
    else tile_row = (screen_pos_row + TILE_SIZE / 2) / TILE_SIZE;
}

// Cập nhật lại vị trí màn hình dựa theo vị trí tile hiện tại
void Object :: update_screen_pos()
{
    screen_pos_col = tile_col * TILE_SIZE;
    screen_pos_row = tile_row * TILE_SIZE;
}

// Thay đổi tốc độ và hướng di chuyển của đối tượng
void Object :: change_speed_and_direction(int _speed_col , int _speed_row , int new_direction)
{
    speed_col = _speed_col;
    speed_row = _speed_row;
    direction = new_direction;
}

// Cho phép đối tượng đi xuyên qua cổng dịch chuyển (ở hai bên map)
void Object :: go_through_portal()
{
    // Vị trí cổng: hàng 14, cột 0 <-> 27
    if(screen_pos_row == 14 * TILE_SIZE) {
        if(direction == LEFT && screen_pos_col == 0 * TILE_SIZE) {
            // Từ bên trái qua phải
            tile_col = 27;
            screen_pos_col = 27 * TILE_SIZE;
        }
        else if(direction == RIGHT && screen_pos_col == 27 * TILE_SIZE) {
            // Từ bên phải qua trái
            tile_col = 0;
            screen_pos_col = 0 * TILE_SIZE;
        }
    }
}

// Gán trạng thái chết cho đối tượng
void Object :: set_dead(bool status , int id)
{
    dead = status;

    // Nếu id == 1 thì không cập nhật lại vị trí (đặc biệt cho pacman chết)
    if(id == 1) return;

    // Cập nhật lại vị trí nếu không phải chết vì va chạm
    update_screen_pos();
}

// Đặt lại vị trí tile và reset trạng thái đối tượng
void Object :: reset_object_tile(const int _tile_col , const int _tile_row)
{
    tile_col = _tile_col;
    tile_row = _tile_row;

    speed_col = 0;
    speed_row = 0;

    update_screen_pos(); // Vị trí màn hình khớp lại với tile

    dead = false;
}

// Trả về trạng thái sống/chết
bool Object :: is_dead() const
{
    return dead;
}

// Getter vị trí tile cột
int Object :: get_tile_col() const
{
    return tile_col;
}

// Getter vị trí tile hàng
int Object :: get_tile_row() const
{
    return tile_row;
}

// Getter vị trí trên màn hình theo trục x
int Object :: get_screen_pos_col() const
{
    return screen_pos_col;
}

// Getter vị trí trên màn hình theo trục y
int Object :: get_screen_pos_row() const
{
    return screen_pos_row;
}
