#pragma once

#ifndef _ITEM_H_
#define _ITEM_H_

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Item
{
    private:

        int tile_col;
        int tile_row;
        bool dead = false;
        SDL_Texture* item_texture = nullptr;

    public:

        Item(SDL_Renderer* &renderer , const std :: string image_path);
        ~Item();

        int get_tile_col() const;

        int get_tile_row() const;

        void spawn_at(const int &_tile_col , const int &_tile_row);

        void destroy_item();

        void render_item(SDL_Renderer* &renderer);

        //
        bool is_destroyed() const;

};

#endif // _ITEM_H_
