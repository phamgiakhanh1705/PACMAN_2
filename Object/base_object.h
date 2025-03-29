#pragma once

#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

class Object
{
    public:

        static const int up = 0;
        static const int right = 1;
        static const int down = 2;
        static const int left = 3;

        static const int object_height = 30;
        static const int object_width = 30;

        static const int tile_size = 16;
        static const int tile_center_offset = tile_size / 2;

        static const int screen_height = 496;
        static const int screen_width = 448;

        static const int portal_tile_pos_x_1 = 0;
        static const int portal_tile_pos_x_2 = 27;

        static const int portal_screen_pos_x_1 = 0;
        static const int portal_screen_pos_x_2 = 432;
        static const int portal_screen_pos_y = 224;

        Object();
        ~Object();

        Object(int new_tile_x , int new_tile_y , int new_velocity_x = 0 , int new_velocity_y = 0);

        void update_screen_pos_from_tile();
        void reset_object_tile(const int new_tile_x , const int new_tile_y);
        void update_velocity_direction(int new_velocity_x , int new_velocity_y , int new_direct);
        void set_dead(bool status , std :: string entity);
        void move();
        void stop();
        void go_through_portal();

        int get_tile_x() const;
        int get_tile_y() const;
        int get_pos_x() const;
        int get_pos_y() const;

        bool is_dead() const;

    private:

        int screen_pos_x;
        int screen_pos_y;

        int tile_x;
        int tile_y;

        int direct = 0;

        bool dead;

    protected:

        int velocity_x = 0;
        int velocity_y = 0;

};

#endif // _BASE_OBJECT_H_
