#include "../Object/texture_source.h"

Texture_source :: Texture_source()
{
    logger = new log_status("texture_source");

    tile_texture = nullptr;
    entity_texture = nullptr;
    ghost_score_texture = nullptr;

    pacman_frame = 0;
    for(int i = 0; i < 7; i++) {
        ghost_frame[i] = 0;
    }
}

Texture_source :: ~Texture_source()
{
    if(tile_texture) {
        SDL_DestroyTexture(tile_texture);
        tile_texture = nullptr;
    }
    if(entity_texture) {
        SDL_DestroyTexture(entity_texture);
        entity_texture = nullptr;
    }
    if(ghost_score_texture) {
        SDL_DestroyTexture(ghost_score_texture);
        ghost_score_texture = nullptr;
    }
    for(int i = 0; i < 7; i++) {
        ghost_frame[i] = 0;
    }

    delete logger;
    logger = nullptr;
}

bool Texture_source :: is_pacman_dead()
{
    if(pacman_frame == 109) {
        pacman_frame = 0;
        return true;
    }
    return false;
}

void Texture_source :: load_tile_texture(SDL_Renderer* &renderer)
{
    SDL_Surface* image = IMG_Load("");
    if(!image) {
        logger -> status(IMG_GetError());
    }
    else {
        tile_texture = SDL_CreateTextureFromSurface(renderer , image);

        int x = 0 , y = 0;
        for(int i = 0; i < 32; i++) {
            tile_sprites[i] = {x , y , 16 , 16};
            y += 17;
            if (i % 4 == 3) {
                x += 17;
                y = 0;
            }
        }
        logger -> status("Tile Texture loaded successfully!");
    }
    SDL_FreeSurface(image);
}

void Texture_source :: render_tile_texture(SDL_Renderer* &renderer , int tile_id , SDL_Rect* dst_rect)
{
    SDL_RenderCopy(renderer , tile_texture , &tile_sprites[tile_id] , dst_rect);
}

void Texture_source :: load_pacman_and_ghost_texture(SDL_Renderer* &renderer)
{
    SDL_Surface* image = IMG_Load("");
    if(!image) {
        logger -> status(IMG_GetError());
    }
    else {
        entity_texture = SDL_CreateTextureFromSurface(renderer , image);

        int pos_tex_x = 0, pos_tex_y = 0;
        for(int i = 0; i < 3; i++) {
            pacman_up[i] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
        }
        for(int i = 3; i < 6; i++) {
            pacman_down[i % 3] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
        }
        for(int i = 6; i < 9; i++) {
            pacman_left[i % 3] = {pos_tex_x, pos_tex_y , 30 , 30};
            pos_tex_x += 31;
        }
        for(int i = 9; i < 12; i++) {
            pacman_right[i % 3] = {pos_tex_x, pos_tex_y, 30, 30};
            pos_tex_x += 31;
        }

        pos_tex_x = 0;
        for (int i = 0; i < 11; i++) {
            pacman_dead[i] = {pos_tex_x , 155 , 30 , 30};
            pos_tex_x += 31;
        }

        logger -> status("Pacman Texture loaded successfully!");

        pos_tex_x = 0;
        pos_tex_y = 31;
        for(int i = 0; i < total_ghost - 1; i++) {
            ghost_sprite[i][up][0] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][up][1] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][down][0] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][down][1] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][left][0] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][left][1] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][right][0] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][right][1] = {pos_tex_x , pos_tex_y , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][frighten_ghost_1][0] = {pos_tex_x , 31 , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][frighten_ghost_1][1] = {pos_tex_x , 31 , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][frighten_ghost_2][0] = {pos_tex_x , 31 , 30 , 30};
            pos_tex_x += 31;
            ghost_sprite[i][frighten_ghost_2][1] = {pos_tex_x , 31 , 30 , 30};
            pos_tex_x = 0;
            pos_tex_y += 31;
            if(i == 3) pos_tex_y += 31;
        }

        pos_tex_x = 248;
        pos_tex_y = 62;
        ghost_sprite[ghost_spirit][up][0] = {pos_tex_x , pos_tex_y , 30 , 30};
        ghost_sprite[ghost_spirit][up][1] = {pos_tex_x , pos_tex_y , 30 , 30};
        pos_tex_x += 31;
        ghost_sprite[ghost_spirit][down][0] = {pos_tex_x , pos_tex_y , 30 , 30};
        ghost_sprite[ghost_spirit][down][1] = {pos_tex_x , pos_tex_y , 30 , 30};
        pos_tex_x = 248;
        pos_tex_y += 31;
        ghost_sprite[ghost_spirit][left][0] = {pos_tex_x , pos_tex_y , 30 , 30};
        ghost_sprite[ghost_spirit][left][1] = {pos_tex_x , pos_tex_y , 30 , 30};
        pos_tex_x += 31;
        ghost_sprite[ghost_spirit][right][0] = {pos_tex_x , pos_tex_y , 30 , 30};
        ghost_sprite[ghost_spirit][right][1] = {pos_tex_x , pos_tex_y , 30 , 30};
        pos_tex_x = pos_tex_y = 0;

        SDL_FreeSurface(image);
        image = IMG_Load("");
        ghost_score_texture = SDL_CreateTextureFromSurface(renderer , image);

        logger -> status("Ghost Texture loaded successfully!");
    }

    SDL_FreeSurface(image);
    image = nullptr;
}

void Texture_source :: render_pacman(SDL_Renderer* &renderer , int pos_x , int pos_y , int status)
{
    SDL_Rect src_rect , dst_rect;

    dst_rect = {pos_x - 7 + 217 , pos_y - 7 , 30 , 30};
    ++pacman_frame;

    if(status != dead_pacman && pacman_frame == 30) {
        pacman_frame = 0;
    }

    switch(status)
    {
        case -1:
            src_rect = pacman_up[0];
            break;
        case up:
            src_rect = pacman_up[pacman_frame / 10];
            break;
        case right:
            src_rect = pacman_right[pacman_frame / 10];
            break;
        case down:
            src_rect = pacman_down[pacman_frame / 10];
            break;
        case left:
            src_rect = pacman_left[pacman_frame / 10];
            break;
        case dead_pacman:
            src_rect = pacman_dead[pacman_frame / 10];
            break;
        default:
            src_rect = pacman_up[0];
            break;
    }

    SDL_RenderCopy(renderer , entity_texture , &src_rect , &dst_rect);
}

void Texture_source :: render_ghost(SDL_Renderer* &renderer , int pos_x , int pos_y , int ghost_id , int status)
{
    SDL_Rect src_rect , dst_rect;
    dst_rect = {pos_x - 7 + 217 , pos_y - 7 , 30 , 30};

    ++ghost_frame[ghost_id];
    if (ghost_frame[ghost_id] == 14) ghost_frame[ghost_id] = 0;

    int frame_index = ghost_frame[ghost_id] / 7;

    switch(status)
    {
        case up:
            src_rect = ghost_sprite[ghost_id][up][frame_index];
            break;
        case right:
            src_rect = ghost_sprite[ghost_id][right][frame_index];
            break;
        case down:
            src_rect = ghost_sprite[ghost_id][down][frame_index];
            break;
        case left:
            src_rect = ghost_sprite[ghost_id][left][frame_index];
            break;
        case frighten_ghost_1:
            src_rect = ghost_sprite[ghost_id][frighten_ghost_1][frame_index];
            break;
        case frighten_ghost_2:
            if(frame_index == 0) {
                src_rect = ghost_sprite[ghost_id][frighten_ghost_1][frame_index];
            }
            else {
                src_rect = ghost_sprite[ghost_id][frighten_ghost_2][frame_index];
            }
            break;
        default:
            src_rect = ghost_sprite[ghost_id][up][0];
            break;
    }

    SDL_RenderCopy(renderer , entity_texture , &src_rect , &dst_rect);
}

void Texture_source :: render_ghost_score(SDL_Renderer* &renderer , int eaten_ghost_x , int eaten_ghost_y , int eaten_ghost_streak)
{
    SDL_Rect src_rect , dst_rect;

    switch(eaten_ghost_streak)
    {
        case 0: src_rect = {0,  0, 50, 50}; break;
        case 1: src_rect = {50, 0, 50, 50}; break;
        case 2: src_rect = {0,  50, 50, 50}; break;
        case 3: src_rect = {50, 50, 50, 50}; break;
        default: src_rect = {0, 0, 50, 50}; break;
    }

    dst_rect = {eaten_ghost_x + 210, eaten_ghost_y - 7, 30, 30};

    SDL_RenderCopy(renderer, ghost_score_texture, &src_rect, &dst_rect);
}

void Texture_source :: render_map(SDL_Renderer* &renderer , Map* map)
{
    int height = map -> get_map_height();
    int width = map -> get_map_width();

    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            int tile_id = map -> get_type_tile(row , col);
            SDL_Rect dst_rect = {col * 16 + 217 , row * 16 , 16 , 16};
            render_tile_texture(renderer , tile_id , &dst_rect);
        }
    }
}

void Texture_source :: render_warning_effect(SDL_Renderer* &renderer)
{
    SDL_SetRenderDrawBlendMode(renderer , SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 60);
    SDL_Rect full_screen = {217 , 0 , 448 , 496};
    SDL_RenderFillRect(renderer , &full_screen);
}

