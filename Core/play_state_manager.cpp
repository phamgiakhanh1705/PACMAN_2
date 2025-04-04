#include "play_state_manager.h"

#include <iostream>

using namespace std;

PlayStateManager :: PlayStateManager()
{
    engine = new Engine();
    pause_menu = new Menu(262 , 170 , (int) pause_menu_button_text.size() , 320 , 30);
    running_menu_pause = false;
}

PlayStateManager :: ~PlayStateManager()
{
    delete engine;
    engine = nullptr;

    running_menu_pause = false;
}

void PlayStateManager :: new_game(SDL_Renderer* &renderer)
{
    static bool init_more_than_once = false;
    if(!init_more_than_once) {
        pause_menu -> init(renderer , "Assets/Menu Image/Pacman Pause Menu.png" , pause_menu_button_text);
        engine -> init_game(renderer);
        init_more_than_once = true;
    }
    else{
        engine -> new_game();
    }
}

void PlayStateManager :: run_game(bool &exit_to_menu)
{
    if(running_menu_pause == false) engine -> loop_game(exit_to_menu);
}

void PlayStateManager :: handle_event(SDL_Event &event , SDL_Renderer* &renderer , bool &exit_to_main_menu , std :: vector <std :: string> &score_data)
{
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running_menu_pause = true;
        pause_menu -> return_main_menu();
        while((int) mute_channel.size() > 0) {
            mute_channel.pop_back();
        }
        for(int channel = 1; channel <= 8; channel++) {
            if(Mix_Paused(channel) == 0) {
                Mix_Pause(channel);
                mute_channel.push_back(channel);
            }
        }
    }
    else {
        if(running_menu_pause) {
            pause_menu -> handle_event(event , renderer);
            if(pause_menu -> get_menu_status() == Menu :: RESUME) {
                running_menu_pause = false;
                for(auto channel : mute_channel) {
                    Mix_Resume(channel);
                }
            }
            if(pause_menu -> get_menu_status() == Menu :: PLAY_BUTTON_PRESSED) {
                running_menu_pause = false;
                engine -> new_game();
            }
            if(pause_menu -> get_menu_status() == Menu :: EXIT_BUTTON_PRESSED) {
                running_menu_pause = false;
                exit_to_main_menu = true;
            }
        }
        else engine -> handle_event_engine(event , score_data);
    }
}

void PlayStateManager :: render(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data)
{
    if(running_menu_pause) pause_menu -> render_menu(renderer , score_data);
    else engine -> render_game(renderer , score_data);
}
