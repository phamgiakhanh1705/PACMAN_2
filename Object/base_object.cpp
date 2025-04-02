#include "base_object.h"
#include <iostream>
#include <cmath>

Object :: Object(int _tile_col , int _tile_row , int _speed_col , int _speed_row)
{
    tile_col = _tile_col;
    tile_row = _tile_row;

    if(speed_col != 0) speed_col = _speed_col;
    if(speed_row != 0) speed_row = _speed_row;

    screen_pos_col = tile_col * TILE_SIZE + TILE_SIZE / 2;
    screen_pos_row = tile_row * TILE_SIZE;

    dead = false;
}

void Object :: move()
{
    screen_pos_col += speed_col;
    screen_pos_row += speed_row;

    if(screen_pos_col % TILE_SIZE == 0) tile_col = screen_pos_col / TILE_SIZE;
    else tile_col = (screen_pos_col + TILE_SIZE / 2) / TILE_SIZE;

    if(screen_pos_row % TILE_SIZE == 0) tile_row = screen_pos_row / TILE_SIZE;
    else tile_row = (screen_pos_row + TILE_SIZE / 2) / TILE_SIZE;
}

void Object :: update_screen_pos()
{
    screen_pos_col = tile_col * TILE_SIZE;
    screen_pos_row = tile_row * TILE_SIZE;
}

void Object :: change_speed_and_direction(int _speed_col , int _speed_row , int new_direction)
{
    speed_col = _speed_col;
    speed_row = _speed_row;
    direction = new_direction;
}

void Object :: go_through_portal()
{
    // portal 1: (0 , 14)
    // portal 2: (27 , 14)
    if(screen_pos_row == 14 * TILE_SIZE) {
        if(direction == LEFT && screen_pos_col == 0 * TILE_SIZE) { // portal 1 -> portal 2
            tile_col = 27;
            screen_pos_col = 27 * TILE_SIZE;
        }
        else if(direction == RIGHT && screen_pos_col == 27 * TILE_SIZE) { // portal 2 -> portal 1
            tile_col = 0;
            screen_pos_col = 0 * TILE_SIZE;
        }
    }
}

void Object :: set_dead(bool status , int id)
{
    dead = status;
    if(id == 1) return;
    update_screen_pos();
}

void Object :: reset_object_tile(const int _tile_col , const int _tile_row)
{
    tile_col = _tile_col;
    tile_row = _tile_row;

    speed_col = 0;
    speed_row = 0;

    update_screen_pos();

    dead = false;
}

bool Object :: is_dead() const
{
    return dead;
}

int Object :: get_tile_col() const
{
    return tile_col;
}

int Object :: get_tile_row() const
{
    return tile_row;
}

int Object :: get_screen_pos_col() const
{
    return screen_pos_col;
}

int Object :: get_screen_pos_row() const
{
    return screen_pos_row;
}
