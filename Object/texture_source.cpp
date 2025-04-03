#include "texture_source.h"

TextureSrc :: TextureSrc()
{
    tile_texture = nullptr;
    entity_texture = nullptr;
    ghost_score = nullptr;
    pacman_frame = 0;
    for(int i = 0; i < 7; i++) ghost_frame[i] = 0;
}

TextureSrc :: ~TextureSrc()
{
    if(tile_texture) {
        SDL_DestroyTexture(tile_texture);
        tile_texture = nullptr;
    }

    if(entity_texture) {
        SDL_DestroyTexture(entity_texture);
        entity_texture = nullptr;
    }

    if(ghost_score) {
        SDL_DestroyTexture(ghost_score);
        ghost_score = nullptr;
    }

    for(int i = 0; i < 7; i++) ghost_frame[i] = 0;
}

bool TextureSrc :: is_pacman_dead()
{
    if(pacman_frame == 109) {
        pacman_frame = 0;
        return true;
    }
    return false;
}

void TextureSrc :: load_tile_texture(SDL_Renderer* &renderer) {
    SDL_Surface* image = IMG_Load("Assets/Entity Image/Pacman Tile Labyrinth.png");

    if(image == nullptr) {
        Console -> status( IMG_GetError() );
    }
    else {
        tile_texture = SDL_CreateTextureFromSurface(renderer , image);

        int col = 0;
        int row = 0;
        for(int i = 0; i < 32; i++) {
            tile_sprite[i] = {col , row , TILE_SIZE , TILE_SIZE};
            row += TILE_SIZE + 1;
            if (i % 4 == 3) {
                col += TILE_SIZE + 1;
                row = 0;
            }
        }

        Console -> status("Tile Texture got successfully!");
    }

    SDL_FreeSurface(image);
    image = nullptr;
}

void TextureSrc :: render_tile_texture(SDL_Renderer* &renderer , int tile_id , SDL_Rect* display_rect)
{
    SDL_RenderCopy(renderer , tile_texture , &tile_sprite[tile_id] , display_rect);
}

void TextureSrc :: load_pacman_and_ghost_texture(SDL_Renderer* &renderer)
{
    SDL_Surface* image = IMG_Load("Assets/Entity Image/Pacman and Ghost Texture.png");

    if(image == nullptr) {
        Console -> status(IMG_GetError());
    }
    else {
        entity_texture = SDL_CreateTextureFromSurface(renderer , image);

        int pos_texture_col = 0;
        int pos_texture_row = 0;

        for(int i = 0; i < 3; i++) {
            pacman_UP[i % 3] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        for(int i = 3; i < 6; i++) {
            pacman_DOWN[i % 3] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        for(int i = 6; i < 9; i++) {
            pacman_LEFT[i % 3] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        for(int i = 9; i < 12; i++) {
            pacman_RIGHT[i % 3] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        pos_texture_col = 0;
        pos_texture_row = 5 * (ENTITY_SIZE + 1);
        for(int i = 0; i < 11; i++) {
            pacman_DEAD[i] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        Console -> status("Pacman Texture got successfully!");

        pos_texture_col = 0;
        pos_texture_row = ENTITY_SIZE + 1;
        for(int i = 0; i < TOTAL_GHOST; i++) {
            pos_texture_col = 0;

            ghost[i][UP][0] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][UP][1] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;

            ghost[i][DOWN][0] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][DOWN][1] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;

            ghost[i][LEFT][0] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][LEFT][1] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;

            ghost[i][RIGHT][0] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][RIGHT][1] = {pos_texture_col , pos_texture_row , ENTITY_SIZE , ENTITY_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;

            pos_texture_row += ENTITY_SIZE + 1;
            if(i == 3) pos_texture_row += ENTITY_SIZE + 1;

        }

        pos_texture_col = 0;
        pos_texture_row = ENTITY_SIZE + 1;
        for(int i = 0; i < TOTAL_GHOST; i++) {
            pos_texture_col = 0;

            ghost[i][FRIGHTEN_GHOST_1][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][FRIGHTEN_GHOST_1][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;

            ghost[i][FRIGHTEN_GHOST_2][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
            ghost[i][FRIGHTEN_GHOST_2][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
            pos_texture_col += ENTITY_SIZE + 1;
        }

        pos_texture_col = 8 * (ENTITY_SIZE + 1);
        pos_texture_row = 2 * (ENTITY_SIZE + 1);

        ghost[GHOST_SPIRIT][UP][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        ghost[GHOST_SPIRIT][UP][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        pos_texture_col += ENTITY_SIZE + 1;

        ghost[GHOST_SPIRIT][DOWN][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        ghost[GHOST_SPIRIT][DOWN][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        pos_texture_col += 8 * (ENTITY_SIZE + 1);
        pos_texture_col += ENTITY_SIZE + 1;

        ghost[GHOST_SPIRIT][LEFT][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        ghost[GHOST_SPIRIT][LEFT][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        pos_texture_col += ENTITY_SIZE + 1;

        ghost[GHOST_SPIRIT][RIGHT][0] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};
        ghost[GHOST_SPIRIT][RIGHT][1] = {pos_texture_col , pos_texture_row , TILE_SIZE , TILE_SIZE};


        SDL_FreeSurface(image);
        image = IMG_Load("Assets/Entity Image/ghostscore.png");
        ghost_score = SDL_CreateTextureFromSurface(renderer , image);

        Console -> status("Ghost Texture got successfully!");
    }

    SDL_FreeSurface(image);
    image = nullptr;
}

void TextureSrc :: render_pacman_texture(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , int status)
{
    SDL_Rect source_rect , display_rect;
    display_rect = {screen_pos_col - 7 + 217 , screen_pos_row - 7 , ENTITY_SIZE , ENTITY_SIZE};
    pacman_frame++;

    if(status != DEAD_PACMAN && pacman_frame == 30) pacman_frame = 0;

    if(status == UP) {
        source_rect = pacman_UP[pacman_frame / 10];
    }
    if(status == DOWN) {
        source_rect = pacman_DOWN[pacman_frame / 10];
    }
    if(status == LEFT) {
        source_rect = pacman_LEFT[pacman_frame / 10];
    }
    if(status == RIGHT) {
        source_rect = pacman_RIGHT[pacman_frame / 10];
    }
    if(status == -1) {
        source_rect = pacman_UP[0];
    }
    if(status == DEAD_PACMAN) {
        source_rect = pacman_DEAD[pacman_frame / 10];
    }

    SDL_RenderCopy(renderer , entity_texture , &source_rect , &display_rect);
}

void TextureSrc :: render_ghost_texture(SDL_Renderer* &renderer , int screen_pos_col , int screen_pos_row , int ghost_id , int status)
{
    SDL_Rect source_rect , display_rect;
    display_rect = {screen_pos_col - 7 + 217 , screen_pos_row - 7 , ENTITY_SIZE , ENTITY_SIZE};
    ghost_frame[ghost_id]++;

    if(ghost_frame[ghost_id] == 14) ghost_frame[ghost_id] = 0;

    if(status == UP) {
        source_rect = ghost[ghost_id][UP][ghost_frame[ghost_id] / 7];
    }
    if(status == DOWN) {
        source_rect = ghost[ghost_id][DOWN][ghost_frame[ghost_id] / 7];
    }
    if(status == LEFT) {
        source_rect = ghost[ghost_id][LEFT][ghost_frame[ghost_id] / 7];
    }
    if(status == RIGHT) {
        source_rect = ghost[ghost_id][RIGHT][ghost_frame[ghost_id] / 7];
    }
    if(status == FRIGHTEN_GHOST_1) {
        source_rect = ghost[ghost_id][FRIGHTEN_GHOST_1][ghost_frame[ghost_id] / 7];
    }
    if(status == FRIGHTEN_GHOST_2) {
        if(ghost_frame[ghost_id] / 7 == 0) {
            source_rect = ghost[ghost_id][FRIGHTEN_GHOST_1][ghost_frame[ghost_id] / 7];
        }
        else {
            source_rect = ghost[ghost_id][FRIGHTEN_GHOST_2][ghost_frame[ghost_id] / 7];
        }
    }

    SDL_RenderCopy(renderer , entity_texture , &source_rect , &display_rect);
}

void TextureSrc :: render_ghost_score(SDL_Renderer* &renderer , const int eaten_ghost_pos_col , const int eaten_ghost_pos_row , const int eaten_ghost_streak)
{
    SDL_Rect source_rect , display_rect;

    if(eaten_ghost_streak == 0) {
        source_rect = {0 , 0 , 50 , 50};
    }
    if(eaten_ghost_streak == 1) {
        source_rect = {50 , 0 , 50 , 50};
    }
    if(eaten_ghost_streak == 2) {
        source_rect = {0 , 50 , 50 , 50};
    }
    if(eaten_ghost_streak == 3) {
        source_rect = {50 , 50 , 50 , 50};
    }

    display_rect = {eaten_ghost_pos_col + 210 , eaten_ghost_pos_row - 7 , 30 , 30};
    SDL_RenderCopy(renderer, ghost_score, &source_rect, &display_rect);
}
