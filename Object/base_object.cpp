#include "../Object/base_object.h"


Object :: Object()
{

}

Object :: ~Object()
{

}

Object :: Object(int new_tile_x , int new_tile_y , int new_velocity_x , int new_velocity_y)
{
    tile_x = new_tile_x;
    tile_y = new_tile_y;
    if(new_velocity_x) velocity_x = new_velocity_x;
    if(new_velocity_y) velocity_y = new_velocity_y;
    screen_pos_x = new_tile_x * tile_size + tile_center_offset;
    screen_pos_y = new_tile_y * tile_size;
    dead = false;
}

void Object :: update_screen_pos_from_tile()
{
    screen_pos_x = tile_x * tile_size;
    screen_pos_y = tile_y * tile_size;
}

void Object ::reset_object_tile(const int new_tile_x , const int new_tile_y)
{
    tile_x = new_tile_x;
    tile_y = new_tile_y;
    screen_pos_x = tile_x * tile_size;
    screen_pos_y = tile_y * tile_size;
    velocity_x = 0;
    velocity_y = 0;
    dead = false;

}

void Object :: update_velocity_direction(int new_velocity_x , int new_velocity_y , int new_direct)
{
    velocity_x = new_velocity_x;
    velocity_y = new_velocity_y;
    direct = new_direct;
}

void Object :: set_dead(bool status , std :: string entity)
{
    dead = status;
    if(entity == "Ghost") {
        screen_pos_x = tile_x * tile_size;
        screen_pos_y = tile_y * tile_size;
    }
}

void Object :: move()
{
    screen_pos_x += velocity_x;
    screen_pos_y += velocity_y;

    if(screen_pos_x % tile_size == 0) {
        tile_x = screen_pos_x / tile_size;
    }
    else {
        tile_x = (screen_pos_x + tile_center_offset) / tile_size;
    }

     if(screen_pos_y % tile_size == 0) {
        tile_y = screen_pos_y / tile_size;
    }
    else {
        tile_y = (screen_pos_y + tile_center_offset) / tile_size;
    }
}

void Object :: stop()
{
    velocity_x = 0;
    velocity_y = 0;
}

void Object :: go_through_portal()
{
    if(screen_pos_y == portal_screen_pos_y) {
        // portal_1 -> portal_2
        if(screen_pos_x == portal_screen_pos_x_1 && direct == left) {
            tile_x = portal_tile_pos_x_2;
            screen_pos_x = portal_screen_pos_x_2;
        }
        // portal_2 -> portal_1
        else if(screen_pos_x == portal_screen_pos_x_2 && direct == right) {
            tile_x = portal_tile_pos_x_1;
            screen_pos_x = portal_screen_pos_x_1;
        }
    }
}

int Object :: get_tile_x() const { return tile_x; }
int Object :: get_tile_y() const { return tile_y; }
int Object :: get_pos_x() const { return screen_pos_x; }
int Object :: get_pos_y() const { return screen_pos_y; }

bool Object :: is_dead() const { return dead; }
