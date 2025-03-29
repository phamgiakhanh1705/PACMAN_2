#pragma once

#ifndef _ITEM_H_
#define _ITEM_H_

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Item
{
    private:

        int tile_x;
        int tile_y;
        bool is_collected = false;
        SDL_Texture* texture = nullptr;

    public:

        Item(SDL_Renderer* &renderer , const std :: string& image_path);
        ~Item();

        int get_tile_x() const;
        int get_tile_y() const;

        bool is_destroyed() const;

        void spawn_at(int new_tile_x , int new_tile_y);
        void destroy();
        void render_item(SDL_Renderer* &renderer);
};
#endif // _ITEM_H_

