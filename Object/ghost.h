#pragma once

#ifndef _GHOST_H_
#define _GHOST_H_

#include "base_object.h"
#include <iostream>

class Ghost : public Object {

    private:

        int target_tile_col;
        int target_tile_row;

        int ghost_direction;
        int ghost_speed;
        int accele;

        bool frighten_mode;
        bool scatter_mode;
        bool in_cage;

    public:

        static const int GHOST_START_TILE_COL = 13;
        static const int GHOST_START_TILE_ROW = 11;

        static const int DEFAULT_BLINKY_TILE_COL = 26;
        static const int DEFAULT_BLINKY_TILE_ROW = 1;
        static const int BLINKY_START_TILE_COL = 13;
        static const int BLINKY_START_TILE_ROW = 11;

        static const int DEFAULT_PINKY_TILE_COL = 1;
        static const int DEFAULT_PINKY_TILE_ROW = 1;
        static const int PINKY_START_TILE_COL = 13;
        static const int PINKY_START_TILE_ROW = 14;

        static const int DEFAULT_INKY_TILE_COL = 26;
        static const int DEFAULT_INKY_TILE_ROW = 29;
        static const int INKY_START_TILE_COL = 11;
        static const int INKY_START_TILE_ROW = 14;

        static const int DEFAULT_CLYDE_TILE_COL = 1;
        static const int DEFAULT_CLYDE_TILE_ROW = 29;
        static const int CLYDE_START_TILE_COL = 15;
        static const int CLYDE_START_TILE_ROW = 14;

        Ghost(int start_tile_col , int start_tile_row , bool _in_cage);
        ~Ghost() {};

        //
        int get_target_tile_col() const;
        //
        int get_target_tile_row() const;
        //
        int get_ghost_direction() const;

        //
        void set_ghost_direction(int direction);
        //
        void set_ghost_frighten(const bool status);
        //
        void set_ghost_scatter(const bool status);
        //
        void set_ghost_destination(int tile_col , int tile_row , int _accele = 1);
        //
        void ghost_moving();
        //
        void ghost_respawn(const int tile_col , const int tile_row , const bool _in_cage);

        //
        bool is_ghost_scatter() const;
        //
        bool is_ghost_frighten() const;
        //
        bool is_ghost_in_cage() const;

};

#endif // _GHOST_H_
