#pragma once
#ifndef MAP_H_

#include "../LogStatus/logstatus.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <SDL.h>

struct position
{
    int x;
    int y;
    constexpr position() : x(0) , y(0) {}
    constexpr position(int _x , int _y) : x(_x) , y(_y) {}
    bool operator == (const position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator != (const position& other) const {
        return x != other.x && y != other.y;
    }
};

class Map
{
    public:
        Map();
        ~Map();

        static const int up = 0;
        static const int right = 1;
        static const int down = 2;
        static const int left = 3;

        bool is_wall(std :: pair <int , int> pos);
        bool is_cross_roads(std :: pair <int , int> pos);
        bool check_range(int x , int y); //
        bool can_change_direction(int x , int y , int new_direction);
        bool beside_cross_is_wall(std :: pair <int , int> cross , int new_direction);

        int get_tile(int x , int y) const;
        int eat_coins(const int &pacman_x , const int &pacman_y);
        int get_distance(std :: pair <int , int> begin , std :: pair <int , int> end , int begin_direction);
        int get_map_width() const;
        int get_map_height() const;
        int get_type_tile(int x , int y) const;

        void reset_map();

        std :: pair <int , int> get_nxt_cross(int x , int y , int direction) const;

    private:
        log_status* console = new log_status("Map"); /// thông tin trạng thái của bản đồ

        const std :: string map_file = "Map/map.txt";
        static const int map_width = 28; // chiều rộng bản đồ
        static const int map_height = 31; // chiều cao bản đồ
        static const int val_1 = 26;
        static const int val_2 = 27;
        static const int val_3 = 30;
        static const int blocks = map_width * map_height; // kích thước bản đồ
        static const int diff_block_types = 31; // số dạng blocks trong map
        static const std :: pair <int , int> portal_1;
        static const std :: pair <int , int> portal_2;
        static const std :: pair <int , int> invalid_point;

        std :: vector <std :: vector <int>> maze;
        std :: vector <int> dist;
        std :: vector <std :: vector <bool>> visited;

        std :: vector <std :: vector <std :: vector <bool>>> is_cross;
        std :: vector <std :: vector <std :: vector <std :: pair <int , int>>>> nxt_cross;

        std :: vector <std :: vector <int>> tile; // các giá trị của các ô
        std :: vector <std :: vector <std :: vector <int>>> distance; // khoảng cách giữa các ô

        void finding_cross_roads();
        void nearest_cross_roads();
        void calc_distance();
};

#endif // MAP_H_


/*
    trong map.txt, các ô:
    1,2,3,4: 4 góc tường của map

*/
