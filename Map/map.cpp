#include "Map.h"
#include <queue>

typedef std :: pair <int , int> II;

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

    map_tile.resize(MAP_HEIGHT , vector <int> (MAP_WIDTH));
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
    for(int row = 0; row < MAP_HEIGHT; row++) {
        for(int col = 0; col < MAP_WIDTH; col++) {

            for(int dir = 0; dir < 4; dir++) {
                mark_cross[row][col][dir] = false;
            }

            if(map_tile[new_row][new_col] != NORMAL_COIN &&
               map_tile[new_row][new_col] != SUPER_COIN  &&
               map_tile[new_row][new_col] != EMPTY_COIN) continue;

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

    if(0 > col && col > MAP_WIDTH - 1) return false;
    if(0 > row && row > MAP_HEIGHT - 1) return false;

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

int Map :: pacman_eat_coins(const int &pacman_tile_col , const int &pacman_tile_row) const
{
    if(tile[pacman_tile_row][pacman_tile_col] == NORMAL_COIN) {
        tile[pacman_tile_row][pacman_tile_col] = EMPTY_COIN;
        return NORMAL_COIN;
    }
    if(tile[pacman_tile_row][pacman_tile_col] == SUPER_COIN) {
        tile[pacman_tile_row][pacman_tile_col] = EMPTY_COIN;
        return SUPER_COIN;
    }
    return 0;
}

int Map :: get_distance(std :: pair <int , int> begin , std :: pair <int , int> end , int start_direction) const
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
    next_cross_id.resize(MAP_HEIGHT , std :: vector <int> (MAP_WIDTH , std :: vector <int> (4)));
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

void Map :: calculateDistance()
{
    int id = 0;
    int dh[4] = {0 , 1 , 0 , -1};
    int dc[4] = {-1 , 0 , 1 , 0};
    dist.resize(MAP_WIDTH * MAP_HEIGHT , std :: vector <std :: vector <int>> (MAP_WIDTH * MAP_HEIGHT , std :: vector <int> (4 , -1)));
    std :: vector <int> dis;
    dis.resize(MAP_WIDTH * MAP_HEIGHT);
    std :: queue <std :: pair <int , int>> visitNode;
    for(int x = 0;x < MAP_WIDTH;++x){
        for(int y = 0;y < MAP_HEIGHT;++y){
            if(isWall(std :: pair <int , int> (x , y))) continue;
            if(y == 14 && (x == 0 || x == 27)) continue;
            for(int startDir = 0;startDir < 4;++startDir){
                int xn = x + dh[startDir] , yn = y + dc[startDir];
                if(isWall(std :: pair <int , int> (xn , yn))) continue;
                for(int i = 0;i < MAP_HEIGHT * MAP_WIDTH;i++) dis[i] = -1;
                i++d;
                color[yn][xn] = id;
                dis[xn * MAP_HEIGHT + yn] = 0;
                visitNode.push(std :: pair <int , int> (yn * MAP_WIDTH + xn , startDir));
                while(!visitNode.empty()){
                    int curx = visitNode.front().first % MAP_WIDTH,
                        cury = visitNode.front().first / MAP_WIDTH,
                        lasDir = visitNode.front().second;
                    visitNode.pop();
                    if(cury == 14 && (curx == 0 || curx == 27)) continue;
                    for(int dir = 0;dir < 4;++dir){
                        int u = curx + dh[dir] , v = cury + dc[dir];
                        if(lasDir % 2 == dir % 2 && dir != lasDir) continue;
                        if(isWall(std :: pair <int , int> (u , v))) continue;
                        if(color[v][u] != id){
                            color[v][u] = id;
                            dis[u * MAP_HEIGHT + v] = dis[curx * MAP_HEIGHT + cury] + 1;
                            visitNode.push(std :: pair <int , int> (v * MAP_WIDTH + u , dir));
                        }
                    }
                }
                for(int i = 0;i < MAP_WIDTH * MAP_HEIGHT;i++)
                    dist[xn * MAP_HEIGHT + yn][i][startDir] = dis[i];
            }
        }
    }
    std :: cout << "Done!";
}
