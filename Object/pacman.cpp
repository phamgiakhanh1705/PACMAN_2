#include "Pacman.h"
#include <iostream>
#include <algorithm>

typedef std :: pair <int , std :: pair <int , int>> IP;

Pacman :: Pacman() : Object(PACMAN_START_TILE_COL , PACMAN_START_TILE_ROW)
{
    while((int) direction.size() > 0) direction.pop();
    while((int) special.size() > 0) special.pop();
}

Pacman :: ~Pacman()
{
    while((int) direction.size() > 0) direction.pop();
    while((int) special.size() > 0) special.pop();
}

void Pacman :: push_to_direction(int new_direction)
{
    if((int) direction.size() > 0) direction.pop();
    direction.push(new_direction);
}

void Pacman :: push_to_special(int new_direction , std :: pair <int , int> next_cross_id)
{
    if((int) special.size() > 0){
        if(special.top().first != new_direction){
            special.pop();
            special.push({new_direction , next_cross_id});
        }
    }
    else special.push({new_direction , next_cross_id});
}

void Pacman :: pacman_moving()
{
    if((int) direction.size() > 0){
        int speed_row = 0;
        int speed_col = 0;
        int current_dir = direction.top();

        if(current_dir == UP) {
            speed_row = (-1) * PACMAN_SPEED;
            speed_col = 0;
        }
        else if(current_dir == DOWN) {
            speed_row = (+1) * PACMAN_SPEED;
            speed_col = 0;
        }
        else if(current_dir == RIGHT) {
            speed_row = 0;
            speed_col = (+1) * PACMAN_SPEED;
        }
        else if(current_dir == LEFT) {
            speed_row = 0;
            speed_col = (-1) * PACMAN_SPEED;
        }
        else current_dir = -1;

        changeVelocityDir(speed_col , speed_row , current_dir);

        move();
    }
}

void Pacman :: pacman_turn()
{
    if((int) direction.size() > 0) pacman_stopmoving();
    direction.push(special.top().first);
    special.pop();
}

void Pacman :: pacman_stopmoving()
{
    while((int) direction.size() > 0) direction.pop();
}

void Pacman :: clear_special()
{
    while((int) special.size() > 0) special.pop();
}

int Pacman :: get_pacman_direction() const
{
    return direction.top();
}

std :: pair <int , int> Pacman :: get_pacman_special_cross_id() const
{
    return special.top().second;
}

bool Pacman :: is_direction_empty() const
{
    return direction.empty();
}

bool Pacman :: is_special_empty() const
{
    return special.empty();
}

void Pacman :: pacman_respawn()
{
    resetObjectTile(PACMAN_START_TILE_COL , PACMAN_START_TILE_ROW);
    while((int) direction.size() > 0) direction.pop();
    while((int) special.size() > 0) special.pop();
}
