#include "Menu.h"
#include <stdio.h>

Menu_manager :: Menu_manager(const int base_scr_pos_x , const int base_scr_pos_y , const int total_button , const int button_width , const int button_height)
{
    menu_texture = nullptr;
    for(int i = 0; i < 3; i++) how_to_play_texture[i] = nullptr;
    high_score_texture = nullptr;
    arrow_texture = nullptr;
    score_text = nullptr;
    arrow_hs_pos = 0;
    total_button_ = total_button;
    menu_button_width = button_width;
    menu_button_height = button_height;
    this -> base_scr_pos_x = base_scr_pos_x;
    this -> base_scr_pos_y = base_scr_pos_y;
}

Menu_manager :: ~Menu_manager()
{
    Mix_FreeChunk(navigation_sound);
    Mix_FreeChunk(selection_sound);

    SDL_DestroyTexture(menu_texture);
    menu_texture = nullptr;
    for(int i = 0; i < 3; i++) {
        SDL_DestroyTexture(how_to_play_texture[i]);
        how_to_play_texture[i] = nullptr;
    }
    SDL_DestroyTexture(high_score_texture);
    high_score_texture = nullptr;
    SDL_DestroyTexture(arrow_texture);
    arrow_texture = nullptr;

    for(Menu_button *button : menu_button) {
        delete button;
        button = nullptr;
    }
    menu_button.clear();
}

SDL_Texture* Menu_manager :: load_image(SDL_Renderer* &renderer , const std :: string img_path)
{
    SDL_Surface *image = IMG_Load(img_path.c_str());
    SDL_Texture *load_texture = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
    return load_texture;
}

void Menu_manager :: init(SDL_Renderer* &renderer , const std :: string img_path , std :: vector <std :: string> &button_text)
{
    SDL_Surface *image = IMG_Load(img_path.c_str());
    if(image == nullptr) console -> status(IMG_GetError());
    else {
        menu_texture = SDL_CreateTextureFromSurface(renderer , image);
        SDL_FreeSurface(image);
        how_to_play_texture[0] = load_image(renderer , "Source/Assets/Menu_manager Image/How to Play1.png");
        how_to_play_texture[1] = load_image(renderer , "Source/Assets/Menu_manager Image/How to Play2.png");
        how_to_play_texture[2] = load_image(renderer , "Source/Assets/Menu_manager Image/How to Play3.png");
        high_score_texture = load_image(renderer , "Source/Assets/Menu_manager Image/High Scores.png");
        arrow_texture = load_image(renderer , "Source/Assets/Menu_manager Image/arrow-upanddown.png");

        score_text = new Text_manager(28);

        for(int i = 0;i < total_button_;i++) {
            menu_button.push_back(new Menu_button(menu_button_width , menu_button_height , base_scr_pos_x - menu_button_width / 2 , base_scr_pos_y + (5 + menu_button_height) * (i - 1)));
        }

        for(int i = 0;i < total_button_;i++) {
            if(button_text[i] == "Sound: ON") {
                if(Mix_Volume(-1 , -1) == 0) button_text[i] = "Sound: OFF";
            }
            menu_button[i] -> load_label(renderer , button_text[i]);
            menu_button[i] -> set_state(Menu_button :: STATE_NORMAL);
        }
        menu_button[0] -> set_state(Menu_button :: STATE_HOVERED);
        current_button_id = 0;
        current_menu_status = RUNNING;
    }
}

void Menu_manager :: render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data)
{
    if(current_menu_status == HOW_TO_PLAY) {
        SDL_RenderCopy(renderer , how_to_play_texture[current_htp_id] , nullptr , nullptr);
    }
    else if(current_menu_status == HIGH_SCORES) {
        SDL_RenderCopy(renderer , high_score_texture , nullptr , nullptr);
        SDL_Rect src_up = {0 , 0 , 431 , 400} , src_down = {431 , 0 , 431 , 400};
        SDL_Rect dst_up = {441 - 41 , 85 , 80 , 80} , dst_down = {441 - 34 , 385 , 80 , 80};
        static bool alpha_up = true;
        if(alpha_up) alpha_mod += 2;
        else alpha_mod -= 2;
        if(alpha_mod > 255) alpha_up = false , alpha_mod = 254;
        else if(alpha_mod < 1) alpha_up = true , alpha_mod = 0;
        SDL_SetTextureAlphaMod(arrow_texture , alpha_mod);
        SDL_RenderCopy(renderer , arrow_texture , &src_up , &dst_up);
        SDL_RenderCopy(renderer , arrow_texture , &src_down , &dst_down);
        for(int i = arrow_hs_pos; i < arrow_hs_pos + 7; i++) {
            score_text -> load_render_text(renderer , score_data[i] , {0 , 0 , 0 , 255});
            score_text -> render_text(renderer , 441 , 160 + 40 * (i - arrow_hs_pos) , Text_manager :: CENTER);
        }
    }
    else {
        SDL_RenderCopy(renderer , menu_texture , nullptr , nullptr);
        for(int i = 0; i < total_button_; i++) menu_button[i] -> button_render(renderer);
    }
}

void Menu_manager :: handle_event(SDL_Event &e , SDL_Renderer* &renderer)
{
    if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_DOWN:
            case SDLK_s:
                Mix_PlayChannel(7 , navigation_sound , 0);
                if(current_menu_status == HIGH_SCORES) {
                    if(arrow_hs_pos < 3) ++arrow_hs_pos;
                }
                else {
                    menu_button[current_button_id] -> set_state(Menu_button :: STATE_NORMAL);
                    (current_button_id += 1) %= total_button_;
                    menu_button[current_button_id] -> set_state(Menu_button :: STATE_HOVERED);
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Mix_PlayChannel(7 , navigation_sound , 0);
                if(current_menu_status == HIGH_SCORES) {
                    if(arrow_hs_pos > 0) --arrow_hs_pos;
                }
                else {
                    menu_button[current_button_id] -> set_state(Menu_button :: STATE_NORMAL);
                    (current_button_id += total_button_ - 1) %= total_button_;
                    menu_button[current_button_id] -> set_state(Menu_button :: STATE_HOVERED);
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                Mix_PlayChannel(7 , navigation_sound , 0);
                if(current_menu_status == HOW_TO_PLAY && current_htp_id > 0) --current_htp_id;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Mix_PlayChannel(7 , navigation_sound , 0);
                if(current_menu_status == HOW_TO_PLAY && current_htp_id < 2) ++current_htp_id;
                break;
            case SDLK_RETURN:
                Mix_PlayChannel(7 , navigation_sound , 0);
                menu_button[current_button_id] -> set_state(Menu_button :: STATE_HOVERED);
                if(current_menu_status == HOW_TO_PLAY || current_menu_status == HIGH_SCORES) {
                    current_menu_status = RUNNING;
                }
                else {
                    std :: string menu_text = menu_button[current_button_id] -> get_label();
                    if(menu_text == "New Game") current_menu_status = PLAY_BUTTON_PRESSED;
                    else if(menu_text == "Resume") current_menu_status = RESUME;
                    else if(menu_text == "Exit" || menu_text == "Exit to Start Menu_manager") current_menu_status = EXIT_BUTTON_PRESSED;
                    else if(menu_text == "Sound: ON") {
                        menu_button[current_button_id] -> change_sound_button(renderer);
                        Mix_Volume(-1 , 0);
                    }
                    else if(menu_text == "Sound: OFF") {
                        menu_button[current_button_id] -> change_sound_button(renderer);
                        Mix_Volume(-1 , MIX_MAX_VOLUME);
                    }
                    else if(menu_text == "How to Play") {
                        current_menu_status = HOW_TO_PLAY;
                        current_htp_id = 0;
                    }
                    else if(menu_text == "High Scores") {
                        current_menu_status = HIGH_SCORES;
                        arrow_hs_pos = 0;
                    }
                }
                break;
        }
    }
}

bool Menu_manager :: is_running() const
{
    return current_menu_status == RUNNING;
}

int Menu_manager :: get_status() const
{
    return current_menu_status;
}

void Menu_manager :: re_open()
{
    current_menu_status = RUNNING;
    menu_button[current_button_id] -> set_state(Menu_button :: STATE_HOVERED);
}

void Menu_manager :: change_run_status()
{
    running = !running;
}
