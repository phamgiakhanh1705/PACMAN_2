#pragma once
#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "../Object/base_object.h"

class Pacman : public Object
{
    public:

        static const int up = 0;
        static const int right = 1;
        static const int down = 2;
        static const int left = 3;
        static const int none_dir = -1; // Pacman đứng yên

        static const int pacman_start_tile_x = 13;
        static const int pacman_start_tile_y = 23;
        static const int pacman_speed = 2;

        Pacman();
        ~Pacman();

        void handle_input(int new_direction);
        void update();

        void plan_turn(int new_direction , std :: pair <int , int> at_tile);
        void perform_planned_turn();

        void stop_moving();
        void clear_turn_plan();
        void respawn();

        int get_current_direction() const;
        std :: pair <int , int> get_planned_turn_tile() const;
        bool has_planned_turn() const;

    private:
        int cur_dir = none_dir; // hướng Pacman đang di chuyển
        int planned_dir = none_dir; // Hướng Pacman sẽ chọn
        std :: pair <int , int> planned_tile = {-1 , -1}; // vị trí tile mà tại đó pacman sẽ chuyển hướng

};

#endif // _PACMAN_H_
