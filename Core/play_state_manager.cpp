#include "play_state_manager.h"
#include <iostream>

PlayStateManager :: PlayStateManager()
{
    engine = new Engine();
    pause_menu = new Menu_manager(262 , 170 , pause_menu_button_text.size() , 320 , 30);
    run_pause_menu = false;
}

PlayStateManager :: ~PlayStateManager()
{
    delete engine;
    engine = nullptr;
    run_pause_menu = false;
}

void PlayStateManager :: new_game(SDL_Renderer* &renderer)
{
    static bool inited = false;
    if(!inited) {
        this -> score_data = score_data;
        pause_menu -> init(renderer , "Source/Assets/Menu Image/Pacman Pause Menu.png" , pause_menu_button_text);
        engine -> init(renderer);
        inited = true;
    } else {
        engine -> new_game();
    }
}

void PlayStateManager :: run_game(bool &exit_to_menu)
{
    if(run_pause_menu == false) engine -> loop(exit_to_menu);
}

void PlayStateManager :: handle_event(SDL_Event &e , SDL_Renderer* &renderer , bool &exit_to_main_menu , std :: vector<std :: string> &score_data)
{
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        run_pause_menu = true;
        pause_menu -> re_open();

        while(!mute_channel.empty()) mute_channel.pop_back();

        for(int channel = 1 ; channel <= 8 ; ++channel) {
            if(Mix_Paused(channel) == 0) {
                Mix_Pause(channel);
                mute_channel.push_back(channel);
            }
        }
    } else {
        if(run_pause_menu) {
            pause_menu -> handle_event(e , renderer);

            switch(pause_menu -> get_status()) {
                case Menu_manager :: RESUME:
                    run_pause_menu = false;
                    for(int channel : mute_channel) Mix_Resume(channel);
                    break;
                case Menu_manager :: PLAY_BUTTON_PRESSED:
                    run_pause_menu = false;
                    engine -> new_game();
                    break;
                case Menu_manager :: EXIT_BUTTON_PRESSED:
                    exit_to_main_menu = true;
                    run_pause_menu = false;
                    break;
            }
        } else {
            engine -> handle_event(e , score_data);
        }
    }
}

void PlayStateManager :: render(SDL_Renderer* &renderer , const std :: vector<std :: string> &score_data)
{
    if(run_pause_menu) {
        pause_menu -> render(renderer , score_data);
    } else {
        engine -> render(renderer , score_data);
    }
}

