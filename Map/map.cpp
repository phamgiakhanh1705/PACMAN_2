#include "Map.h"
#include <queue>

const std :: pair <int , int> Map :: INVALID_POINT = {-1 , -1};
const std :: pair <int , int> Map :: PORTAL_1_TILE_ID = {0 , 14};
const std :: pair <int , int> Map :: PORTAL_2_TILE_ID = {27 , 14};

const int Map :: UP = 0;
const int Map :: RIGHT = 1;
const int Map :: DOWN = 2;
const int Map :: LEFT = 3;

const int drow[] = {-1 , 0 , 1 , 0};
const int dcol[] = {0 , 1 , 0 , -1};

bool first_init = true;

Map :: Map()
{
    if(first_init == true) {
        first_init = false;
        std :: ifstream file(MAP_FILE);
        if(file.is_open()){
            original_map.resize(MAP_HEIGHT , std :: vector <int> (MAP_WIDTH));
            for(int row = 0; row < MAP_HEIGHT; row++){
                for(int col = 0; col < MAP_WIDTH; col++){
                    file >> original_map[row][col];
                    if(original_map[row][col] > 31 || original_map[row][col] < 0){
                        console -> status("Wrong Tile!");
                        return;
                    }
                }
            }
            console -> status("Map read successfully!");
        }
        else console -> status("Error reading file!");
    }

    map_tile.resize(MAP_HEIGHT , std :: vector <int> (MAP_WIDTH));
    for(int row = 0; row < MAP_HEIGHT; row++) {
        for(int col = 0; col < MAP_WIDTH; col++) {
            map_tile[row][col] = original_map[row][col];
        }
    }

    finding_all_cross_tile_id();
    finding_nearest_cross_tile_id();
    calc_distance();
}

void Map :: finding_all_cross_tile_id()
{
    mark_cross.resize(MAP_HEIGHT , std :: vector <std :: vector <bool>> (MAP_WIDTH , std :: vector <bool> (4)));
    for(int row = 0; row < MAP_HEIGHT; row++) {
        for(int col = 0; col < MAP_WIDTH; col++) {

            for(int dir = 0; dir < 4; dir++) {
                mark_cross[row][col][dir] = false;
            }

            if(map_tile[row][col] != NORMAL_COIN &&
               map_tile[row][col] != SUPER_COIN  &&
               map_tile[row][col] != EMPTY_COIN) continue;

            for(int dir = 0; dir < 4; dir++) {

                int new_row = row + drow[dir];
                int new_col = col + dcol[dir];

                if(0 <= new_row && new_row <= MAP_HEIGHT - 1 &&
                   0 <= new_col && new_col <= MAP_WIDTH - 1)
                {
                    if(map_tile[new_row][new_col] == NORMAL_COIN ||
                       map_tile[new_row][new_col] == SUPER_COIN  ||
                       map_tile[new_row][new_col] == EMPTY_COIN)
                    {
                        mark_cross[row][col][dir] = true;
                    }
                }
            }
        }
    }
}

bool Map :: is_tile_cross(int col , int row)
{
    int cnt = 0;
    if(mark_cross[row][col][UP] == true) cnt++;
    if(mark_cross[row][col][RIGHT] == true) cnt++;
    if(mark_cross[row][col][DOWN] == true) cnt++;
    if(mark_cross[row][col][LEFT] == true) cnt++;

    if(cnt > 2) return true;
    if(cnt == 2) {
        if(mark_cross[row][col][UP] == true && mark_cross[row][col][DOWN] == true) return false;
        if(mark_cross[row][col][RIGHT] == true && mark_cross[row][col][LEFT] == true) return false;
        return true;
    }
    if(cnt < 2) return false;
}

bool Map :: is_beside_cross_is_wall(std :: pair <int , int> cross , int new_direction)
{
    int &col = cross.first;
    int &row = cross.second;

    if(new_direction == UP) row--;
    if(new_direction == DOWN) row++;
    if(new_direction == LEFT) col--;
    if(new_direction == RIGHT) col++;

    return is_wall(cross);
}

bool Map :: is_wall(std :: pair <int , int> tile_id)
{
    if(tile_id == PORTAL_1_TILE_ID || tile_id == PORTAL_2_TILE_ID) return false;

    int col = tile_id.first;
    int row = tile_id.second;

    if(0 > col || col > MAP_WIDTH - 1) return true;
    if(0 > row || row > MAP_HEIGHT - 1) return true;

    if(map_tile[row][col] == NORMAL_COIN) return false;
    if(map_tile[row][col] == SUPER_COIN) return false;
    if(map_tile[row][col] == EMPTY_COIN) return false;

    return true;
}

bool Map :: can_change_direction(int col , int row , int new_direction)
{
    return mark_cross[row][col][new_direction];
}

int Map :: get_tile_id(int col , int row) const
{
    return map_tile[row][col];
}

std :: pair <int , int> Map :: get_nearest_cross_tile_id(int col , int row , int direction) const
{
    return next_cross_id[row][col][direction];
}

int Map :: pacman_eat_coins(const int &pacman_tile_col , const int &pacman_tile_row)
{
    if(map_tile[pacman_tile_row][pacman_tile_col] == NORMAL_COIN) {
        map_tile[pacman_tile_row][pacman_tile_col] = EMPTY_COIN;
        return NORMAL_COIN;
    }
    if(map_tile[pacman_tile_row][pacman_tile_col] == SUPER_COIN) {
        map_tile[pacman_tile_row][pacman_tile_col] = EMPTY_COIN;
        return SUPER_COIN;
    }
    return 0;
}

int Map :: get_distance(std :: pair <int , int> begin , std :: pair <int , int> end , int start_direction)
{
    int euclid_distance = (begin.first - end.first) * (begin.first - end.first) + (begin.second - end.second) * (begin.second - end.second);
    if(is_wall(end) == true) {
        return euclid_distance;
    }
    else {
        if(distance[begin.first * MAP_HEIGHT + begin.second][end.first * MAP_HEIGHT + end.second][start_direction] == -1) {
            return euclid_distance;
        }
        return distance[begin.first * MAP_HEIGHT + begin.second][end.first * MAP_HEIGHT + end.second][start_direction];
    }
}

void Map :: map_reset()
{
    for(int row = 0; row < MAP_HEIGHT; row++) {
        for(int col = 0; col < MAP_WIDTH; col++) {
            map_tile[row][col] = original_map[row][col];
        }
    }
}

void Map :: finding_nearest_cross_tile_id()
{
    next_cross_id.resize(MAP_HEIGHT , std :: vector <std :: vector <std :: pair <int , int>>> (MAP_WIDTH , std :: vector <std :: pair <int , int>> (4)));
    for(int row = 0; row < MAP_HEIGHT; row++) {

        next_cross_id[row][0][LEFT] = INVALID_POINT;
        for(int col = 1; col < MAP_WIDTH; col++) {
            next_cross_id[row][col][LEFT] = INVALID_POINT;
            if(is_wall({col , row}) == true) continue;
            if(next_cross_id[row][col - 1][LEFT] != INVALID_POINT) {
                next_cross_id[row][col][LEFT] = next_cross_id[row][col - 1][LEFT];
            }
            if(is_tile_cross(col - 1 , row) == true) {
                next_cross_id[row][col][LEFT] = {col - 1 , row};
            }
        }

        next_cross_id[row][MAP_WIDTH - 1][RIGHT] = INVALID_POINT;
        for(int col = MAP_WIDTH - 2; col >= 0; col--) {
            next_cross_id[row][col][RIGHT] = INVALID_POINT;
            if(is_wall({col , row}) == true) continue;
            if(next_cross_id[row][col + 1][RIGHT] != INVALID_POINT) {
                next_cross_id[row][col][RIGHT] = next_cross_id[row][col + 1][RIGHT];
            }
            if(is_tile_cross(col + 1 , row) == true) {
                next_cross_id[row][col][RIGHT] = {col + 1 , row};
            }
        }
    }

    for(int col = 0; col < MAP_WIDTH; col++) {

        next_cross_id[0][col][UP] = INVALID_POINT;
        for(int row = 1; row < MAP_HEIGHT; row++) {
            next_cross_id[row][col][UP] = INVALID_POINT;
            if(is_wall({col , row}) == true) continue;
            if(next_cross_id[row - 1][col][UP] != INVALID_POINT) {
                next_cross_id[row][col][UP] = next_cross_id[row - 1][col][UP];
            }
            if(is_tile_cross(col , row - 1) == true) {
                next_cross_id[row][col][UP] = {col , row - 1};
            }
        }

        next_cross_id[MAP_HEIGHT - 1][col][DOWN] = INVALID_POINT;
        for(int row = MAP_HEIGHT - 2; row >= 0; row--) {
            next_cross_id[row][col][DOWN] = INVALID_POINT;
            if(is_wall({col , row}) == true) continue;
            if(next_cross_id[row + 1][col][DOWN] != INVALID_POINT) {
                next_cross_id[row][col][DOWN] = next_cross_id[row + 1][col][DOWN];
            }
            if(is_tile_cross(col , row + 1) == true) {
                next_cross_id[row][col][DOWN] = {col , row + 1};
            }
        }
    }
}

void Map :: calc_distance()
{
    int cnt = 0;
    distance.resize(MAP_HEIGHT * MAP_WIDTH , std :: vector <std :: vector <int>> (MAP_HEIGHT * MAP_WIDTH , std :: vector <int> (4 , -1)));
    visited_node.resize(MAP_HEIGHT , std :: vector <int> (MAP_WIDTH , 0));
    std :: vector <int> dist(MAP_HEIGHT * MAP_WIDTH);
    std :: queue <std :: pair <std :: pair <int , int> , int > > current_node;

    for(int col = 0; col < MAP_WIDTH; col++) {
        for(int row = 0; row < MAP_HEIGHT; row++) {

            if(is_wall({col , row})) continue;
            if(std :: make_pair(col , row) == PORTAL_1_TILE_ID || std :: make_pair(col , row) == PORTAL_2_TILE_ID) continue;

            for(int start_dir = 0; start_dir < 4; start_dir++) {

                int start_row = row + drow[start_dir];
                int start_col = col + dcol[start_dir];

                if(is_wall({start_col , start_row})) continue;

                for(int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) dist[i] = -1;
                visited_node[start_row][start_col] = ++cnt;
                dist[start_col * MAP_HEIGHT + start_row] = 0;
                current_node.push({{start_col , start_row} , start_dir});

                while((int) current_node.size() > 0) {

                    int current_col = current_node.front().first.first;
                    int current_row = current_node.front().first.second;
                    int last_dir = current_node.front().second;
                    current_node.pop();

                    if(std :: make_pair(current_col , current_row) == PORTAL_1_TILE_ID) continue;
                    if(std :: make_pair(current_col , current_row) == PORTAL_2_TILE_ID) continue;

                    for(int current_dir = 0; current_dir < 4; current_dir++) {

                        int new_col = current_col + dcol[current_dir];
                        int new_row = current_row + drow[current_dir];

                        if(is_wall({new_col , new_row})) continue;
                        if((current_dir == UP && last_dir == DOWN) || (current_dir == DOWN && last_dir == UP)) continue;
                        if((current_dir == RIGHT && last_dir == LEFT) || (current_dir == LEFT && last_dir == RIGHT)) continue;
                        if(visited_node[new_row][new_col] == cnt) continue;

                        visited_node[new_row][new_col] = cnt;
                        dist[new_col * MAP_HEIGHT + new_row] = dist[current_col * MAP_HEIGHT + current_row] + 1;
                        current_node.push({{new_col , new_col} , current_dir});
                    }
                }

                for(int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
                    distance[start_col * MAP_HEIGHT + start_col][i][start_dir] = dist[i];
                }
            }
        }
    }
}

int Map :: get_map_width() const { return MAP_WIDTH; }
int Map :: get_map_height() const { return MAP_HEIGHT; }
