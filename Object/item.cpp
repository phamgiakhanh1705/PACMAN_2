#include "../Object/Item.h"

Item :: Item(SDL_Renderer* &renderer , const std :: string& image_path)
{
    tile_x = 0;
    tile_y = 0;
    is_collected = true;

    SDL_Surface* surface = IMG_Load(image_path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer , surface);
    SDL_FreeSurface(surface);
}

Item :: ~Item()
{
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

int Item :: get_tile_x() const
{
    return tile_x;
}

int Item :: get_tile_y() const
{
    return tile_y;
}

bool Item :: is_destroyed() const
{
    return is_collected;
}

void Item :: spawn_at(int new_tile_x , int new_tile_y)
{
    tile_x = new_tile_x;
    tile_y = new_tile_y;
    is_collected = false;
}

void Item :: destroy()
{
    is_collected = true;
}

void Item :: render_item(SDL_Renderer* &renderer)
{
    if (is_collected) return;

    SDL_Rect render_rect = {
        tile_x * 16 + 210 ,
        tile_y * 16 - 7 ,
        30 ,
        30
    };

    SDL_RenderCopy(renderer , texture , nullptr , &render_rect);
}
