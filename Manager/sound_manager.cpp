#include "../Manager/sound_manager.h"

Sound_manager :: Sound_manager()
{
    for(int i = 0; i < total_sounds; i++) {
            sounds[i] = nullptr;
    }
    dot_timer = 0;
    prev_move_sound = -1;
    curr_move_sound = step_0;
    ghost_frightened = false;
    ghost_escaping = false;
    is_dead = false;
    logger = new log_status("Sound Manager");
}

Sound_manager :: ~Sound_manager()
{
    for(int i = 0; i < total_sounds; i++) {
        if(sounds[i]) {
            Mix_FreeChunk(sounds[i]);
        }
        sounds[i] = nullptr;
    }
    delete logger;
}

void Sound_manager :: queue_sound(int sound_id)
{
    switch(sound_id)
    {
        case chomp:
            dot_timer = 16;
            break;
        case ghost_eaten:
            Mix_PlayChannel(4 , sounds[ghost_eaten] , 0);
            break;
        case step_0: case step_1: case step_2: case step_3:
            curr_move_sound = sound_id;
            break;
        case ghost_home:
            ghost_escaping = true;
            break;
        case revive_ghost:
            ghost_escaping = false;
            break;
        case ghost_turn_blue:
            ghost_frightened = true;
            break;
        case normal_ghost:
            ghost_frightened = false;
            break;
        case pacman_die:
            is_dead = true;
            Mix_Pause(6); ghost_escaping = false;
            Mix_Pause(5); ghost_frightened = false;
            Mix_Pause(3); dot_timer = 0;
            Mix_Pause(1);
            break;
        case start:
        case win_level:
            is_dead = false;
            Mix_Pause(6); ghost_escaping = false;
            Mix_Pause(5); ghost_frightened = false;
            Mix_Pause(3); dot_timer = 0;
            Mix_Pause(1);
            prev_move_sound = (sound_id == win_level ? step_0 : -1);
            Mix_PlayChannel(2 , sounds[sound_id] , 0);
            break;
        default:
            break;
    }
}

void Sound_manager :: load_sound()
{
    sounds[step_0] = Mix_LoadWAV("");
    sounds[step_1] = Mix_LoadWAV("");
    sounds[step_2] = Mix_LoadWAV("");
    sounds[step_3] = Mix_LoadWAV("");
    sounds[start]  = Mix_LoadWAV("");
    sounds[pacman_die] = Mix_LoadWAV("");
    sounds[chomp] = Mix_LoadWAV("");
    sounds[ghost_eaten] = Mix_LoadWAV("");
    sounds[win_level] = Mix_LoadWAV("");
    sounds[ghost_turn_blue] = Mix_LoadWAV("");
    sounds[ghost_home] = Mix_LoadWAV("");
}

void Sound_manager :: play_sound()
{
    // channel 2 - DEAD
    if(is_dead) {
        Mix_PlayChannel(2 , sounds[pacman_die] , 0);
        is_dead = false;
    }

    if(Mix_Playing(2)) return;

    // channel 1 - MOVE
    if(curr_move_sound != prev_move_sound) {
        Mix_PlayChannel(1 , sounds[curr_move_sound] , -1);
        prev_move_sound = curr_move_sound;
    }

    // channel 3 - EAT_DOT
    if(dot_timer > 0) {
        --dot_timer;
        Mix_Resume(3);
    } else {
        Mix_Pause(3);
    }

    // channel 5 - GHOST_TURN_BLUE
    if(ghost_turn_blue) {
        Mix_Resume(5);
    } else {
        Mix_Pause(5);
    }

    // channel 6 - GHOST_GO_HOME
    if(ghost_escaping) {
        Mix_Resume(6);
        if(ghost_turn_blue) Mix_Pause(5);
    } else {
        Mix_Pause(6);
        if(ghost_turn_blue) Mix_Resume(5);
    }
}


void Sound_manager :: clear_status()
{
    is_dead = false;
    ghost_frightened = false;
    ghost_escaping = false;
    dot_timer = 0;
    prev_move_sound = -1;
    curr_move_sound = step_0;
}
