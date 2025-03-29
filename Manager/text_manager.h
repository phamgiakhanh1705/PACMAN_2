#pragma once

#ifndef _TEXT_MANAGER_H_
#define _TEXT_MANAGER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

class Text_manager
{
    private:

        SDL_Rect display_rect; // vùng chữ nhật định vị vị trí và kích thước của vùng hiển thị chữ
        TTF_Font* font; // con trỏ dùng để trỏ đến font chữ dùng để hiển thị
        SDL_Texture* text_texture; // texture chứa phần chữ sau khi được tạo
        SDL_Surface* text_surface; // surface chứa hình ảnh chữ trước khi convert sang texture

    public:

        Text_manager(int font_size); // constructor , truyền vào cỡ chữ
        ~Text_manager(); // destructor , giải phóng bộ nhớ

        const std :: string FONT_NAME = "..."; // đường dẫn tới font chữ dùng
        static const int CENTER = 0; // căn giữa
        static const int LEFT = 1; // căn trái

        // lấy chiều rộng của dòng text
        int get_text_width() const;

        // tạo texture chữ từ nội dung text và màu text_color
        void load_render_text(SDL_Renderer*& renderer , std :: string text , SDL_Color text_color);
        
        // hiển thị chữ ra màn hình tại vị trí (x , y) với kiểu căn giữa/trái
        void render_text(SDL_Renderer*& renderer , int x , int y , const int type);
};

#endif // _TEXT_MANAGER_H_
