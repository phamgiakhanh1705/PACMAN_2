#include "ghost.h"
#include <random>
#include <iostream>

Ghost :: Ghost(int start_tile_col , int start_tile_row , bool _in_cage) : Object(start_tile_col , start_tile_row)
{
    frighten_mode = 0;
    accele = 1;
    ghost_speed = 2;
    scatter_mode = false;
    target_tile_col = start_tile_col;
    target_tile_row = start_tile_row;
    in_cage = _in_cage;
    if(in_cage == false) ghost_direction = RIGHT;
    else ghost_direction = UP;
}

int Ghost :: get_target_tile_col() const
{
    return target_tile_col;
}

int Ghost :: get_target_tile_row() const
{
    return target_tile_row;
}

int Ghost :: get_ghost_direction() const
{
    return ghost_direction;
}

void Ghost :: set_ghost_direction(int direction)
{
    ghost_direction = direction;
}

void Ghost :: set_ghost_frighten(const bool status)
{
    if(is_ghost_in_cage()) return;
    if(frighten_mode != status) reTilePos();
    frighten_mode = status;
    if(status) {
        ghost_direction = (ghost_direction + 2) % 4;
        accele = 1;
    }
}

void Ghost :: set_ghost_scatter(const bool status)
{
    scatter_mode = status;
}

bool Ghost :: is_ghost_scatter() const
{
    return scatter_mode;
}

bool Ghost :: is_ghost_frighten() const
{
    return frighten_mode;
}

void Ghost :: set_ghost_destination(int tile_col , int tile_row , int _accele)
{
    accele = _accele;
    target_tile_col = tile_col;
    target_tile_row = tile_row;
}

void Ghost :: ghost_moving()
{
    if(accele == 1) {
        if(isDead()) ghost_speed = 4;
        else if(is_ghost_frighten()) ghost_speed = 1;
        else ghost_speed = 2;
    }
    else ghost_speed = 4;

    int speed_col = 0;
    int speed_row = 0;
    int dir = -1;

    if(ghost_direction == UP) {
        speed_row = (-1) * ghost_speed;
        speed_col = 0;
        dir = UP;
    }
    if(ghost_direction == DOWN) {
        speed_row = (+1) * ghost_speed;
        speed_col = 0;
        dir = DOWN;
    }
    if(ghost_direction == RIGHT) {
        speed_row = 0;
        speed_col = (+1) * ghost_speed;
        dir = RIGHT;
    }
    if(ghost_direction == LEFT) {
        speed_row = 0;
        speed_col = (-1) * ghost_speed;
        dir = LEFT;
    }

    changeVelocityDir(speed_col , speed_row , dir);
    move();
}

void Ghost :: ghost_respawn(const int tile_col, const int tile_row, const bool _in_cage)
{
    resetObjectTile(tile_col , tile_row);
    in_cage = _in_cage;
    if(in_cage == false) {
        if(rand() % 2 == 0) ghost_direction = LEFT;
        else ghost_direction = RIGHT;
    }
    else ghost_direction = UP;
}

bool Ghost :: is_ghost_in_cage() const
{
    return in_cage;
}
