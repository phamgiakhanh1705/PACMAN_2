#include "tick_manager.h"
#include <iostream>

const int Tick_manager :: MODE_FRIGHTENED;
const int Tick_manager :: MODE_SCATTER;
const int Tick_manager :: MODE_CHASE;
constexpr double Tick_manager :: TIME_INFINITY;
constexpr double Tick_manager :: FRIENDY_CHASE_DURATION;
constexpr double Tick_manager :: GREENDY_CHASE_DURATION;

Tick_manager :: Tick_manager()
{
    last_tick = SDL_GetTicks();
    frightened_tick = greendy_tick = SDL_GetTicks();

    mode_timeline.push_back(mode_entry_t(MODE_CHASE , TIME_INFINITY));
    mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
    mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
    mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
    mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
    mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
    mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
    mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
}

bool Tick_manager :: is_scatter_time() const
{
    return mode_timeline.back().first == MODE_SCATTER;
}

void Tick_manager :: activate_frightened_mode()
{
    last_status = mode_timeline.back().first;
    mode_timeline.pop_back();

    double time_remaining = 0;
    if(last_status == MODE_CHASE)
        time_remaining = chase_duration - (SDL_GetTicks() - last_tick);
    else if(last_status == MODE_SCATTER)
        time_remaining = scatter_duration - (SDL_GetTicks() - last_tick);

    mode_timeline.push_back(mode_entry_t(last_status , time_remaining));
    mode_timeline.push_back(mode_entry_t(MODE_FRIGHTENED , frightened_duration));

    last_tick = SDL_GetTicks();
}

bool Tick_manager :: is_friendy_chase_time() const
{
    double time_passed = (SDL_GetTicks() - frightened_tick) / 1000.0;
    return time_passed <= FRIENDY_CHASE_DURATION;
}

bool Tick_manager :: is_greendy_chase_time() const
{
    double time_passed = (SDL_GetTicks() - greendy_tick) / 1000.0;
    return time_passed <= GREENDY_CHASE_DURATION;
}

void Tick_manager :: start_friendy_chase()
{
    frightened_tick = SDL_GetTicks();
}

void Tick_manager :: start_greendy_chase()
{
    greendy_tick = SDL_GetTicks();
}

void Tick_manager :: reset_tick(const int level)
{
    mode_timeline.clear();

    if(level == 1) {
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , TIME_INFINITY));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        frightened_duration = 5.0;
    }
    else if(level < 5) {
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , TIME_INFINITY));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , 1.0));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , 1033.0));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , 5.0));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        frightened_duration = 3.0;
    }
    else {
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , TIME_INFINITY));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , 1.0));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , 1037.0));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , 5.0));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        mode_timeline.push_back(mode_entry_t(MODE_CHASE , chase_duration));
        mode_timeline.push_back(mode_entry_t(MODE_SCATTER , scatter_duration));
        frightened_duration = 1.0;
    }

    last_tick = SDL_GetTicks();
}

void Tick_manager :: update_mode_status()
{
    if(is_paused) {
        last_tick = SDL_GetTicks();
        return;
    }

    double time_passed = (SDL_GetTicks() - last_tick) / 1000.0;

    if((int) mode_timeline.size() > 0) {
        if(time_passed > mode_timeline.back().second && mode_timeline.back().second != TIME_INFINITY) {
            mode_timeline.pop_back();
            last_tick = SDL_GetTicks();
        }
    }
}

void Tick_manager :: stabilize_fps()
{
    if(SDL_GetTicks() - last_frame < 1000 / FPS) {
        SDL_Delay(1000 / FPS + last_frame - SDL_GetTicks());
    }
    last_frame = SDL_GetTicks();
}

bool Tick_manager :: toggle_pause(const bool status)
{
    is_paused = status;
    return is_paused;
}

bool Tick_manager :: is_frightened_time() const
{
    return mode_timeline.back().first == MODE_FRIGHTENED;
}

double Tick_manager :: get_remaining_frightened_time() const
{
    return frightened_duration - (SDL_GetTicks() - last_tick) / 1000.0;
}

