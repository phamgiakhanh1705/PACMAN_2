#pragma once

#ifndef _GHOST_H_
#define _GHOST_H_

#include "../Object/base_object.h"
#include <iostream>

class Ghost : public Object
{
    public:

        static const int default_speed = 2;
        static const int frightened_speed = 1;
        static const int returning_speed = 4;

        static const int ghost_start_tile_x = 13;
        static const int ghost_start_tile_y = 11;

        static const int default_blinky_tile_x = 26;
        static const int default_blinky_tile_y = 1;
        static const int blinky_start_tile_x = 13;
        static const int blinky_start_tile_y = 11;

        static const int default_pinky_tile_x = 1;
        static const int default_pinky_tile_y = 1;
        static const int pinky_start_tile_x = 13;
        static const int pinky_start_tile_y = 14;

        static const int default_inky_tile_x = 26;
        static const int default_inky_tile_y = 29;
        static const int inky_start_tile_x = 11;
        static const int inky_start_tile_y = 14;

        static const int default_clyde_tile_x = 1;
        static const int default_clyde_tile_y = 29;
        static const int clyde_start_tile_x = 15;
        static const int clyde_start_tile_y = 14;

        Ghost();
        ~Ghost();

        Ghost(int start_tile_x , int start_tile_y , bool start_in_cage);

        void update_movement();
        void set_direction(int new_direction);
        void reverse_direction();

        void set_frightened_mode(const bool status);
        bool get_frightened_state() const;

        void set_scatter_mode(bool enable);
        bool get_scatter_mode() const;

        void set_target_tile(int tile_x , int tile_y , int multiplier = 1);
        int get_target_tile_x() const;
        int get_target_tile_y() const;
        int get_ghost_direction() const;

        void ghost_respawn(int start_tile_x , int start_tile_y , bool to_cage);
        bool inside_cage() const;

    private:

        int target_tile_x;
        int target_tile_y;
        int direct;

        bool is_frightened;
        bool is_in_scatter_mode;
        bool is_in_cage;

        int ghost_speed;
        int speed_multiplier;
};

#endif // _GHOST_H_
