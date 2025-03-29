
#include "../Map/map.h"
#include <queue>

const int dx[] = {-1 , 0 , 1 , 0};
const int dy[] = {0 , 1 , 0 , -1};

const std :: pair <int , int> Map :: portal_1 = {0 , 14};
const std :: pair <int , int> Map :: portal_2 = {0 , 27};
const std :: pair <int , int> Map :: invalid_point = {-1 , -1};

#define inf INT_MAX

Map :: Map()
{
    std :: ifstream file(map_file);
    if(file.is_open()) {
        maze.resize(map_height , std :: vector <int> (map_width));
        for(int i = 0; i < map_height; i++) {
            for(int j = 0; j < map_width; j++) {
                file >> maze[i][j];
                if(0 > maze[i][j] || maze[i][j] > diff_block_types) {
                    console -> status("Ivalid Tile!");
                    return;
                }
            }
        }
        console -> status("Successfull reading map!");
    }
    else {
        console -> status("Failed reading file!");
    }

    tile.resize(map_height , std :: vector <int> (map_width));
    for(int i = 0; i < map_height; i++) {
        for(int j = 0; j < map_width; j++) {
            tile[i][j] = maze[i][j];
        }
    }

    finding_cross_roads(); // tìm điểm giao cắt (ngã tư)
    nearest_cross_roads(); // tìm điểm giao cắt gần nhất đối với các ô
    calc_distance(); // độ dài đường đi của các ô với nhau
}

Map :: ~Map()
{
    console = nullptr;
    delete console;
}

bool Map :: check_range(int x , int y)
{
    return (0 <= x && x <= map_height - 1 && 0 <= y && y <= map_width - 1);
}

void Map :: finding_cross_roads()
{
    is_cross.resize(map_height , std :: vector <std :: vector <bool>> (map_width , std :: vector <bool> (4)));
//    for(int i = 0; i < map_height; i++) {
//        for(int j = 0; j < map_width; j++) {
//            for(int k = 0; k < 4; k++) {
//                is_cross[i][j][k] = false;
//            }
//        }
//    }

    for(int i = 0; i < map_height; i++) {
        for(int j = 0; j < map_width; j++) {
                for(int k = 0; k < 4; k++) {
                    is_cross[i][j][k] = false;
                }
            if(tile[i][j] == val_1 || tile[i][j] == val_2 || tile[i][j] == val_3) {
                for(int k = 0; k < 4; k++) {
                    int new_i = i + dx[k];
                    int new_j = j + dy[k];
                    if(0 < new_i && new_i < map_height - 1 && 0 < new_j && new_j < map_width &&
                       (tile[new_i][new_j] == val_1 || tile[new_i][new_j] == val_2 || tile[new_i][new_j] == val_3)) {
                        is_cross[i][j][k] = true;
                    }
                }
            }
        }
    }
}

bool Map :: is_wall(std :: pair <int , int> pos)
{
    if(pos == portal_1 || pos == portal_2) return false;

    int x = pos.first;
    int y = pos.second;
    if(x <= 0 || x >= map_width - 1) return true;
    if(y <= 0 || y >= map_height - 1)  return true;

    if(tile[y][x] == val_1 || tile[y][x] == val_2 || tile[y][x] == val_3) return false;
    return true;
}

bool Map :: is_cross_roads(std :: pair <int , int> pos)
{
    int x = pos.first;
    int y = pos.second;

    int cnt = 0;
    if(is_cross[x][y][left] == true) cnt++;
    if(is_cross[x][y][right] == true) cnt++;
    if(is_cross[x][y][up] == true) cnt++;
    if(is_cross[x][y][down] == true) cnt++;

    if(cnt > 2) {
        return true;
    }
    if(cnt == 2) {
        if(is_cross[x][y][left] == true && is_cross[x][y][right] == true) return false;
        if(is_cross[x][y][up] == true && is_cross[x][y][down] == true) return false;
        return true;
    }
    return false;
}

std :: pair <int , int> Map :: get_nxt_cross(int x , int y , int direction) const
{
    return nxt_cross[x][y][direction];
}

void Map :: nearest_cross_roads()
{
    nxt_cross.resize(map_height , std :: vector <std :: vector <std :: pair <int , int>>> (map_width , std :: vector <std :: pair <int , int>> (4)));
    for(int i = 0; i < map_height; i++) {

        // cột biên trái
        nxt_cross[i][0][up] = invalid_point;
        nxt_cross[i][0][right] = invalid_point;
        nxt_cross[i][0][down] = invalid_point;
        nxt_cross[i][0][left] = invalid_point;

        // cột biên phải
        nxt_cross[i][map_width - 1][up] = invalid_point;
        nxt_cross[i][map_width - 1][right] = invalid_point;
        nxt_cross[i][map_width - 1][down] = invalid_point;
        nxt_cross[i][map_width - 1][left] = invalid_point;

    }

    for(int j = 0; j < map_width; j++) {

        // cột biên trên
        nxt_cross[0][j][up] = invalid_point;
        nxt_cross[0][j][right] = invalid_point;
        nxt_cross[0][j][down] = invalid_point;
        nxt_cross[0][j][left] = invalid_point;

        // cột biên dưới
        nxt_cross[map_height - 1][j][up] = invalid_point;
        nxt_cross[map_height - 1][j][right] = invalid_point;
        nxt_cross[map_height - 1][j][down] = invalid_point;
        nxt_cross[map_height - 1][j][left] = invalid_point;

    }

    // chạy ưu tiên trái sang phải
    for(int i = 0; i < map_height; i++) {

        // LEFT
        for(int j = 1; j < map_width; j++) {
            nxt_cross[i][j][left] = invalid_point;
            if(is_wall(std :: make_pair(i , j)) == false) {
                if(nxt_cross[i][j - 1][left] != invalid_point) {
                    nxt_cross[i][j][left] = nxt_cross[i][j - 1][left];
                }
                if(is_cross_roads(std :: make_pair(i , j - 1)) == true) {
                    nxt_cross[i][j][left] = std :: make_pair(i , j - 1);
                }
            }
        }

        // RIGHT
        for(int j = map_width - 2; j >= 0; j--) {
            nxt_cross[i][j][right] = invalid_point;
            if(is_wall(std :: make_pair(i , j)) == false) {
                if(nxt_cross[i][j + 1][right] != invalid_point) {
                    nxt_cross[i][j][right] = nxt_cross[i][j + 1][right];
                }
                if(is_cross_roads(std :: make_pair(i , j + 1)) == true) {
                    nxt_cross[i][j][right] = std :: make_pair(i , j + 1);
                }
            }
        }
    }

    // chạy ưu tiên từ trên xuống
    for(int j = 0; j < map_width; j++) {

        // UP
        for(int i = 1; i < map_height; i++) {
            nxt_cross[i][j][up] = invalid_point;
            if(is_wall(std :: make_pair(i , j)) == false) {
                if(nxt_cross[i - 1][j][up] != invalid_point) {
                    nxt_cross[i][j][up] = nxt_cross[i - 1][j][up];
                }
                if(is_cross_roads(std :: make_pair(i - 1 , j)) == true) {
                    nxt_cross[i][j][up] = std :: make_pair(i - 1 , j);
                }
            }
        }

        // DOWN
        for(int i = map_height - 2; i >= 0; i--) {
            nxt_cross[i][j][down] = invalid_point;
            if(is_wall(std :: make_pair(i , j)) == false) {
                if(nxt_cross[i + 1][j][down] != invalid_point) {
                    nxt_cross[i][j][down] = nxt_cross[i + 1][j][down];
                }
                if(is_cross_roads(std :: make_pair(i + 1 , j)) == true) {
                    nxt_cross[i][j][down] = std :: make_pair(i + 1 , j);
                }
            }
        }
    }
}

bool Map :: can_change_direction(int x , int y , int new_direction)
{
    return is_cross[y][x][new_direction];
}

bool Map :: beside_cross_is_wall(std :: pair <int , int> cross , int new_direction)
{
    auto[x , y] = cross;
    switch(new_direction) {
        case up: x--; break;
        case down: x++; break;
        case right: y++; break;
        case left: y--; break;
        default: break;
    }
    return is_wall(std :: make_pair(x , y));
}

int Map :: eat_coins(const int &pacman_x , const int &pacman_y)
{
    if(tile[pacman_x][pacman_y] == 26) {
        tile[pacman_x][pacman_y] = 30;
        return 26;
    }
    if(tile[pacman_x][pacman_y] == 27) {
        tile[pacman_x][pacman_y] = 30;
        return 27;
    }
    return 0;
}

int Map :: get_distance(std :: pair <int , int> begin , std :: pair <int , int> end , int begin_direction)
{
    if(is_wall(end) == true) {
        return (begin.first - end.first) * (begin.first - end.first) + (begin.second - end.second) * (begin.second - end.second);
    }
    else {
        if(distance[begin.first * map_width + begin.second][end.first * map_width + end.second][begin_direction] == -1) {
            return (begin.first - end.first) * (begin.first - end.first) + (begin.second - end.second) * (begin.second - end.second);
        }
        else {
            return distance[begin.first * map_width + begin.second][end.first * map_width + end.second][begin_direction];
        }
    }
    return 0;
}


void Map :: calc_distance()
{
    distance.resize(blocks , std :: vector <std :: vector <int>> (blocks , std :: vector <int> (4 , inf)));
    dist.resize(blocks);
    visited.resize(map_height , std :: vector <bool> (map_width));


//    for(int i = 0; i < map_height; i++) {
//        for(int j = 0; j < map_width; j++) {
//            for(int u = 0; u < map_height; u++) {
//                for(int v = 0; v < map_width; v++) {
//                    for(int k = 0; k < 4; k++) {
//                        distance[i * map_height + j][u * map_height + v][k] = inf;
//                    }
//                }
//            }
//        }
//    }

    for(int i = 0; i < map_height; i++) {
        for(int j = 0; j < map_width; j++) {
            if(is_wall(std :: make_pair(i , j)) == true) continue;
            if(std :: make_pair(i , j) == portal_1 || std :: make_pair(i , j) == portal_2) continue;
            std :: queue <std :: pair <std :: pair <int , int> , int>> q;
            for(int k = 0; k < 4; k++) {
                int x = i + dx[k];
                int y = j + dy[k];
                if(check_range(x , y) == false) continue;
                if(is_wall(std :: make_pair(x , y)) == true) continue;
                for(int i = 0; i < map_height; i++) {
                    for(int j = 0; j < map_width; j++) {
                        visited[i][j] = false;
                    }
                }
                for(int i = 0; i < blocks; i++) {
                    dist[i] = inf;
                }
                dist[x * map_width + y] = 0;
                visited[x][y] = true;
                q.push({std :: make_pair(x , y) , k});
                while((int) q.size() > 0) {
                    int cur_x = q.front().first.first;
                    int cur_y = q.front().first.second;
                    int pre_direct = q.front().second;
                    q.pop();
                    int cur_block = cur_x * map_width + cur_y;
                    if(std :: make_pair(cur_x , cur_y) == portal_1) continue;
                    if(std :: make_pair(cur_x , cur_y) == portal_2) continue;
                    for(int direct = 0; direct < 4; direct++) {
                        int u = cur_x + dx[direct];
                        int v = cur_y + dy[direct];
                        if(check_range(u , v) == false) continue;
                        if((pre_direct == left && direct == right) ||
                           (pre_direct == right && direct == left) ||
                           (pre_direct == up && direct == down) ||
                           (pre_direct == down && direct == up)) continue;
                        if(is_wall(std :: make_pair(u , v)) == true) continue;
                        if(visited[u][v] == true) continue;
                        visited[u][v] = true;
                        dist[u * map_width + v] = dist[cur_block] + 1;
                        q.push({std :: make_pair(u , v) , direct});
                    }
                }
                for(int i = 0; i < blocks; i++) {
                    distance[x * map_width + y][i][k] = dist[i];
                }
            }
        }
    }
}

int Map :: get_tile(int x , int y) const { return tile[x][y]; }
int Map :: get_map_height() const { return map_height; }
int Map :: get_map_width() const { return map_width; }
int Map :: get_type_tile(int row , int col) const { return tile[row][col]; }

void Map :: reset_map()
{
    for(int row = 0; row < map_height; row++) {
        for(int col = 0; col < map_width; col++) {
            tile[row][col] = maze[row][col];
        }
    }
}
