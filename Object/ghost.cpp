#include "../Object/ghost.h"


Ghost :: Ghost(int start_tile_x , int start_tile_y , bool start_in_cage)
    : Object(start_tile_x , start_tile_y),
      is_frightened(false) ,
      is_in_scatter_mode(false) ,
      is_in_cage(start_in_cage) ,
      speed_multiplier(1) ,
      ghost_speed(default_speed)
{
    target_tile_x = start_tile_x;
    target_tile_y = start_tile_y;
    if(is_in_cage == true) {
        direct = up;
    }
    else {
        direct = right;
    }
}

Ghost :: ~Ghost()
{

}

void Ghost ::update_movement()
{
    int vel_x = 0;
    int vel_y = 0;

    if(speed_multiplier == 1) {
        if(is_frightened == true) {
            ghost_speed = frightened_speed;
        }
        else if(is_dead() == true) {
            ghost_speed = returning_speed;
        }
        else {
            ghost_speed = default_speed;
        }
    }
    else {
        ghost_speed = returning_speed;
    }

    switch(direct)
    {
        case up:
            vel_y = -ghost_speed;
            break;
        case down:
            vel_y = ghost_speed;
            break;
        case left:
            vel_x = -ghost_speed;
            break;
        case right:
            vel_x = ghost_speed;
            break;
        default:
            break;
    }

    update_velocity_direction(vel_x , vel_y , direct);
    move();
}

void Ghost :: set_direction(int new_direction)
{
    direct = new_direction;
}

void Ghost :: reverse_direction()
{
    direct = (direct + 2) % 4;
}

void Ghost :: set_frightened_mode(const bool status)
{
    if(is_in_cage == true) return;
    if(is_frightened == false) update_screen_pos_from_tile();
    is_frightened = status;
    if(status == false) return;
    reverse_direction();
    speed_multiplier = 1;
}

bool Ghost :: get_frightened_state() const
{
    return is_frightened;
}

void Ghost :: set_scatter_mode(bool enable)
{
    is_in_scatter_mode = enable;
}

bool Ghost :: get_scatter_mode() const
{
    return is_in_scatter_mode;
}

void Ghost :: set_target_tile(int tile_x , int tile_y , int multiplier)
{
    target_tile_x = tile_x;
    target_tile_y = tile_y;
    speed_multiplier = multiplier;
}

int Ghost :: get_target_tile_x() const
{
    return target_tile_x;
}

int Ghost :: get_target_tile_y() const
{
    return target_tile_y;
}

int Ghost :: get_ghost_direction() const
{
    return direct;
}

void Ghost :: ghost_respawn(int start_tile_x , int start_tile_y , bool to_cage)
{
    reset_object_tile(start_tile_x , start_tile_y);
    is_in_cage = to_cage;
    if(to_cage == true) {
        direct = up;
    }
    else {
        direct = (rand() % 2 == 0 ? right : left);
    }
}

bool Ghost :: inside_cage() const
{
    return is_in_cage;
}
