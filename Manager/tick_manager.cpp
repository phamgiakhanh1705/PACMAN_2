#include "tick_manager.h"
#include <iostream>

TickManager :: TickManager()
{
    last_tick = SDL_GetTicks();
    flash_tick = glast_tick = SDL_GetTicks();
    current_mode.push(CID(CHASING_MODE , inf));
    current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
    current_mode.push(CID(CHASING_MODE , CHASING_TIME));
    current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
    current_mode.push(CID(CHASING_MODE , CHASING_TIME));
    current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
    current_mode.push(CID(CHASING_MODE , CHASING_TIME));
    current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
}

bool TickManager :: is_scatter_time() const
{
    return current_mode.top().first == SCATTER_MODE;
}

bool TickManager :: is_frighten_time() const
{
    return current_mode.top().first == FRIGHTEN_MODE;
}

double TickManager :: remain_frighten_time() const
{
    return FRIGHTEN_TIME - (SDL_GetTicks() - last_tick) / 1000.0;
}

bool TickManager :: is_friendy_chase_time() const
{
    double time_pass = (SDL_GetTicks() - flash_tick) / 1000.0;
    return (time_pass <= FRIENDY_CHASE_TIME);
}

bool TickManager :: is_greeny_chase_time() const
{
    double time_pass = (SDL_GetTicks() - glast_tick) / 1000.0;
    return (time_pass <= GREENDY_CHASE_TIME);
}

void TickManager :: friendy_start_chase_pacman()
{
    flash_tick = SDL_GetTicks();
}

void TickManager :: greendy_start_chase_pacman()
{
    glast_tick = SDL_GetTicks();
}

bool TickManager :: pause_tick(const bool status)
{
    return pause = status;
}

void TickManager :: update_status()
{
    if(pause == true){
        last_tick = SDL_GetTicks();
        return;
    }
    double time_pass = (SDL_GetTicks() - last_tick) / 1000.0;
    if((int) current_mode.size() > 0){
        if(time_pass > current_mode.top().second && current_mode.top().second != inf){
            last_tick = SDL_GetTicks();
            current_mode.pop();
        }
    }
}

void TickManager :: stablize_FPS()
{
    if(SDL_GetTicks() - last_frame < 1000 / FPS){
        SDL_Delay(1000 / FPS + last_frame - SDL_GetTicks());
    }
    last_frame = SDL_GetTicks();
}

void TickManager :: set_frighten_time()
{
    last_status = current_mode.top().first;
    current_mode.pop();
    if(last_status == CHASING_MODE) {
        current_mode.push(CID(CHASING_MODE , CHASING_TIME - (SDL_GetTicks() - last_tick)));
    }
    else if(last_status == SCATTER_MODE) {
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME - (SDL_GetTicks() - last_tick)));
    }
    current_mode.push(CID(FRIGHTEN_MODE , FRIGHTEN_TIME));
    last_tick = SDL_GetTicks();
}

void TickManager :: reset_tick(const int current_level)
{
    while((int) current_mode.size() > 0) current_mode.pop();
    if(current_level == 1) {
        current_mode.push(CID(CHASING_MODE , inf));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        FRIGHTEN_TIME = 5.0;
    }
    else if(current_level < 5) {
        current_mode.push(CID(CHASING_MODE , inf));
        current_mode.push(CID(SCATTER_MODE , 1.0));
        current_mode.push(CID(CHASING_MODE , 1033.0));
        current_mode.push(CID(SCATTER_MODE , 5.0));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        FRIGHTEN_TIME = 3.0;
    }
    else {
        current_mode.push(CID(CHASING_MODE , inf));
        current_mode.push(CID(SCATTER_MODE , 1.0));
        current_mode.push(CID(CHASING_MODE , 1037.0));
        current_mode.push(CID(SCATTER_MODE , 5.0));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        current_mode.push(CID(CHASING_MODE , CHASING_TIME));
        current_mode.push(CID(SCATTER_MODE , SCATTER_TIME));
        FRIGHTEN_TIME = 1.0;
    }
    last_tick = SDL_GetTicks();
}
