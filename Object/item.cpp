#include "Item.h"

Item :: Item(SDL_Renderer* &renderer , const std :: string image_path)
{
    tile_col = 0;
    tile_row = 0;
    dead = true;
    SDL_Surface* image = IMG_Load(image_path.c_str());
    item_texture = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
}

Item :: ~Item()
{
    SDL_DestroyTexture(item_texture);
    item_texture = nullptr;
}

int Item :: get_tile_col() const
{
    return tile_col;
}

int Item :: get_tile_row() const
{
    return tile_row;
}

bool Item :: is_destroyed() const
{
    return dead;
}

void Item :: spawn_at(const int &_tile_col , const int &_tile_row)
{
    tile_col = _tile_col;
    tile_row = _tile_row;
    dead = false;
}

void Item :: destroy_item()
{
    dead = true;
}

void Item :: render_item(SDL_Renderer* &renderer)
{
    SDL_Rect dsRect = {tile_col * 16 + 210 , tile_row * 16 - 7 , 30 , 30};
    SDL_RenderCopy(renderer , item_texture , nullptr , &dsRect);
}
