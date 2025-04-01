#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "../LogStatus/logstatus.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

class Map
{
    private:

        static const int MAP_HEIGHT = 31; // Chiều ngang của map
        static const int MAP_WIDTH = 28; // Chiều cao của map
        const std :: string MAP_FILE = "Map/map.txt"; // Chứa dữ liệu của bản đồ gốc

        static const int NORMAL_COIN = 26;
        static const int SUPER_COIN = 27;
        static const int EMPTY_COIN = 30;

        static const int INVALID_POINT = {-1 , -1};

        log_status* console = new log_status("Map");

        std :: vector <std :: vector <int> > map_tile; // Map hiện tại
        std :: vector <std :: vector <int> > original_map; // Map đọc từ file gốc
        std :: vector <std :: vector <int> > visited_node; // Đánh dấu những ô đã được thăm
        std :: vector <std :: vector <std :: vector <int> > > distance; // khoảng cách từ các ô tới nhau
        std :: vector <std :: pair <int , int> > next_cross_id; // Lưu điểm rẽ gân nhất
        std :: vector <std :: vector <std :: vector <bool> > > mark_cross; // Đánh dấu xem ô hiện tại có sang được 4 hướng hay không

        // Tìm tất cả điểm rẽ trong map
        void finding_all_cross_tile_id();
        // Tính khoảng cách của các cặp điểm
        void calc_distance();
        // Tìm điểm rẽ gần nhất từ một ô nào đó
        void finding_nearest_cross_tile_id();

    public:

        // Các hướng đi
        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;

        static const std :: pair <int , int> PORTAL_1_TILE_ID = {0 , 14};
        static const std :: pair <int , int> PORTAL_2_TILE_ID = {27 , 14};

        Map();

        ~Map() {
            console = nullptr;
            delete console;
        }

        // Trả về giá trị tại ô đó, xem ô đó là loại nào(wall,coin,...)
        int get_tile_id(int x , int y) const;
        // Cập nhật giá trị tile khi pacman ăn coin
        int pacman_eat_coins(const int &pacman_tile_x , const int &pacman_tile_y) const;
        // Trả về khoảng cách từ begin -> end
        int get_distance(std :: pair <int , int> begin , std :: pair <int , int> end , int start_direction) const;
        // Trả về ngã rẽ gần nhất nếu đi từ ô (x,y) với hướng đi direction
        std :: pair <int , int> get_nearest_cross_tile_id(int x , int y , int direction) const;

        // Kiểm tra xem ô này có phải là tường không
        bool is_wall(std :: pair <int , int> tile_id);
        // Kiểm tra xem ô này có phải ngã rẽ không
        bool is_tile_cross(int y , int x);
        // Kiểm tra xem ô (x,y) có đổi hướng sang new_direction được không
        bool can_change_direction(int x , int y , int new_direction);
        // Kiểm tra xem cạnh ngã rẽ có phải là tường với hướng đi new_direction không
        bool is_beside_cross_is_wall(std :: pair <int , int> cross , int new_direction);
        // Khôi phục lại bản đồ trạng thái ban đầu (dùng khi chơi lại)
        void map_reset();

};

#endif // _MAP_H_
