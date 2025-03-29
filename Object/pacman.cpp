#include "../Object/pacman.h"

Pacman :: Pacman() : Object(pacman_start_tile_x , pacman_start_tile_y)
{
    stop_moving();
    clear_turn_plan();
}

Pacman :: ~Pacman()
{

}

void Pacman :: handle_input(int new_direction)
{
    planned_dir = new_direction;
}

void Pacman :: update()
{
    int tile_x = get_tile_x();
    int tile_y = get_tile_y();
    if(tile_x == planned_tile.first && tile_y == planned_tile.second) {
        perform_planned_turn();
    }

    int vel_x = 0;
    int vel_y = 0;

    switch(cur_dir)
    {
        case up:
            vel_y = -pacman_speed;
            break;
        case down:
            vel_y = pacman_speed;
            break;
        case left:
            vel_x = -pacman_speed;
            break;
        case right:
            vel_x = pacman_speed;
            break;
        default:
            break;
    }

    update_velocity_direction(vel_x , vel_y , cur_dir);
    move();
}

void Pacman :: plan_turn(int new_direction , std :: pair <int , int> at_tile)
{
    planned_dir = new_direction;
    planned_tile = at_tile;
}

void Pacman :: perform_planned_turn()
{
    if(planned_dir != none_dir) {
        cur_dir = planned_dir;
        planned_dir = none_dir;
        planned_tile = std :: make_pair(-1 , -1);
    }
}

void Pacman :: stop_moving()
{
    cur_dir = none_dir;
    velocity_x = 0;
    velocity_y = 0;
}

void Pacman :: clear_turn_plan()
{
    planned_dir = none_dir;
    planned_tile = std :: make_pair(-1 , -1);
}

void Pacman :: respawn()
{
    reset_object_tile(pacman_start_tile_x , pacman_start_tile_y);
    stop_moving();
    clear_turn_plan();
}

int Pacman :: get_current_direction() const
{
    return cur_dir;
}

std :: pair <int , int> Pacman :: get_planned_turn_tile() const
{
    return planned_tile;
}

bool Pacman :: has_planned_turn() const
{
    return (planned_dir != none_dir);
}
