#include "menu.h"
#include <stdio.h>

Menu_manager :: Menu_manager()
{
    background_texture = nullptr;
    highscore_texture = nullptr;
    instruction_texture = nullptr;
    arrow_texture = nullptr;
    highscore_title_text = nullptr;
    arrow_highscore_pos = 0;
    selected_menu_button = 0;
    menu_state = STATE_MENU;
    show_arrow = true;
}

Menu_manager :: ~Menu_manager()
{
    Mix_FreeChunk(click_sound);

    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(highscore_texture);
    SDL_DestroyTexture(instruction_texture);
    SDL_DestroyTexture(arrow_texture);

    delete highscore_title_text; highscore_title_text = nullptr;

    for(Menu_button* btn : menu_buttons) delete btn;
    for(Menu_button* btn : highscore_buttons) delete btn;
    for(Menu_button* btn : instruction_buttons) delete btn;

    menu_buttons.clear();
    highscore_buttons.clear();
    instruction_buttons.clear();
}

SDL_Texture* load_texture(SDL_Renderer* &renderer , const std :: string &path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if(!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer , surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Menu_manager :: initialize(SDL_Renderer* &renderer)
{
    background_texture = load_texture(renderer , "Source/Assets/Menu Image/mainmenu.png");
    highscore_texture  = load_texture(renderer , "Source/Assets/Menu Image/highscore.png");
    instruction_texture= load_texture(renderer , "Source/Assets/Menu Image/howtoplay.png");
    arrow_texture      = load_texture(renderer , "Source/Assets/Menu Image/arrow.png");
    click_sound        = Mix_LoadWAV("Source/Assets/Sound/button.wav");

    menu_buttons.push_back(new Menu_button(100 , 40 , 80 , 120));
    menu_buttons.push_back(new Menu_button(100 , 40 , 80 , 180));
    menu_buttons.push_back(new Menu_button(100 , 40 , 80 , 240));

    menu_buttons[0] -> load_label(renderer , "Play");
    menu_buttons[1] -> load_label(renderer , "High Score");
    menu_buttons[2] -> load_label(renderer , "Exit");

    for(int i = 0 ; i < (int) menu_buttons.size() ; ++i) {
        if(i == selected_menu_button) menu_buttons[i] -> set_state(Menu_button :: STATE_HOVERED);
        else menu_buttons[i] -> set_state(Menu_button :: STATE_NORMAL);
    }

    highscore_title_text = new Text_manager(24);
    highscore_title_text -> load_render_text(renderer , "High Scores:" , {255 , 255 , 255 , 255});
}

void Menu_manager :: render(SDL_Renderer* &renderer)
{
    if(menu_state == STATE_MENU)
        SDL_RenderCopy(renderer , background_texture , nullptr , nullptr);
    else if(menu_state == STATE_HIGHSCORE)
        SDL_RenderCopy(renderer , highscore_texture , nullptr , nullptr);
    else if(menu_state == STATE_INSTRUCTION)
        SDL_RenderCopy(renderer , instruction_texture , nullptr , nullptr);

    if(menu_state == STATE_MENU) {
        for(auto &btn : menu_buttons) btn -> button_render(renderer);

        if(show_arrow) {
            SDL_Rect arrow_rect = {60 , 130 + selected_menu_button * 60 , 20 , 20};
            SDL_RenderCopy(renderer , arrow_texture , nullptr , &arrow_rect);
        }
    }

    if(menu_state == STATE_HIGHSCORE) {
        highscore_title_text -> render_text(renderer , 200 , 40 , Text_manager :: CENTER);
    }
}

void Menu_manager :: handle_event(SDL_Event &event , bool &running , bool &play_game)
{
    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_DOWN:
            case SDLK_s:
                selected_menu_button = (selected_menu_button + 1) % menu_buttons.size();
                Mix_PlayChannel(-1 , click_sound , 0);
                break;

            case SDLK_UP:
            case SDLK_w:
                selected_menu_button = (selected_menu_button - 1 + menu_buttons.size()) % menu_buttons.size();
                Mix_PlayChannel(-1 , click_sound , 0);
                break;

            case SDLK_RETURN:
                Mix_PlayChannel(-1 , click_sound , 0);
                if(selected_menu_button == 0) play_game = true;
                else if(selected_menu_button == 1) menu_state = STATE_HIGHSCORE;
                else if(selected_menu_button == 2) running = false;
                break;

            case SDLK_ESCAPE:
                if(menu_state != STATE_MENU) menu_state = STATE_MENU;
                break;
        }
    }
}

int Menu_manager :: get_menu_state() const
{
    return menu_state;
}

std :: vector<std :: string> Menu_manager :: get_highscore_data() const
{
    return highscore_data;
}

