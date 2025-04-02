#pragma once

#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "base_object.h"
#include <stack>

class Pacman : public Object
{
    public:

        static const int PACMAN_START_TILE_COL = 13;
        static const int PACMAN_START_TILE_ROW = 23;
        static const int PACMAN_SPEED = 2;

        Pacman();
        ~Pacman();

        //
        bool is_direction_empty() const;
        //
        bool is_special_empty() const;

        //
        void push_to_direction(int new_direction);
        //
        void push_to_special(int new_direction , std :: pair <int , int> next_cross_id);
        //
        void pacman_moving();
        //
        void pacman_stopmoving();
        //
        void pacman_turn();
        //
        void clear_special();
        //
        void pacman_respawn();

        //
        int get_pacman_direction() const;
        //
        std :: pair <int , int> get_pacman_special_cross_id() const;

    private:

        std :: stack <int> direction;
        std :: stack <std :: pair <int , std :: pair <int , int> > > special;
};

#endif // _PACMAN_H_
