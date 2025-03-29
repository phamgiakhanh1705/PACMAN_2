#include "text_manager.h"

Text_manager :: Text_manager(int font_size)
{
    font = TTF_OpenFont(FONT_NAME.c_str() , font_size);

    // khởi tạo texture và surface
    text_texture = nullptr;
    text_surface = nullptr;
}

Text_manager :: ~Text_manager()
{
    // nếu font đã được mở thì đóng
    if(font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    // nếu texture đã được tạo thì hủy
    if(text_texture != nullptr) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    // nếu surface đã được tạo thì giải phóng
    if(text_surface != nullptr) {
        SDL_FreeSurface(text_surface);
        text_surface = nullptr;
    }
}

// trả về chiều rộng của chữ đã renderer
int Text_manager :: get_text_width() const
{
    return text_surface -> w;
}

// tạo ra chữ bằng text và text_color , rồi chuyển thành texture để in lên màn hình
void Text_manager :: load_render_text(SDL_Renderer* &renderer , std :: string text , SDL_Color text_color)
{
    // nếu surface đã được tạo thì giải phóng
    if(text_surface != nullptr) {
        SDL_FreeSurface(text_surface);
        text_surface = nullptr;
    }

    // render chữ ra surface với text và màu text_color
    text_surface = TTF_RenderText_Blended(font , text.c_str() , text_color);

    // nếu texture đã được tạo thì hủy
    if(text_texture != nullptr) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    // chuyển từ surface sang texture để in lên màn hình bằng renderer
    text_texture = SDL_CreateTextureFromSurface(renderer , text_surface);
}

// vẽ ra chữ đã được render tại ô (x , y) , đồng thời hỗ trợ căn trái hoặc căn giữa
void Text_manager :: render_text(SDL_Renderer* &renderer , int x , int y , const int type)
{
    // thiết lập vùng chữ nhật nơi chữ sẽ được vẽ với kích thước bằng surface
    display_rect = {x , y , text_surface -> w , text_surface -> h};

    // căn giữa
    if(type == CENTER) {
        display_rect.x = x - text_surface -> w / 2;
        display_rect.y = y - text_surface -> h / 2;
    }

    // vẽ texture lên renderer tại vị trí display_rect
    SDL_RenderCopy(renderer , text_texture , nullptr , &display_rect);
}
