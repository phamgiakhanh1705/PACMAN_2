#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <algorithm>

class Object
{

    private:

        int screen_pos_col;
        int screen_pos_row;

        int tile_col;
        int tile_row;

        int speed_col;
        int speed_row;

        int direction;
        bool dead;

    public:

        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;

        static const int OBJECT_WIDTH  = 30;
        static const int OBJECT_HEIGHT = 30;
        static const int SCREEN_WIDTH = 448;
        static const int SCREEN_HEIGHT = 496;
        static const int TILE_SIZE = 16;

        Object(int _tile_col , int _tile_row , int _speed_col = 0 , int _speed_row = 0);
        ~Object() {}

        //
        int get_screen_pos_col() const;
        //
        int get_screen_pos_row() const;
        //
        int get_tile_col() const;
        //
        int get_tile_row() const;

        //
        void move();
        //
        void update_screen_pos();
        //
        void change_speed_and_direction(int _speed_col, int _speed_row, int new_direction);
        //
        void set_dead(bool status , int id = 0);
        //
        void reset_object_tile(const int _tile_col , const int _tile_row);
        //
        void go_through_portal();

        //
        bool is_dead() const;
};

#endif // _OBJECT_H_
