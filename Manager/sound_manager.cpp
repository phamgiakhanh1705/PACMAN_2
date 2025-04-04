#include "sound_manager.h"
#include <iostream>

using namespace std;

SoundManager :: SoundManager()
{
    for(int i = 0; i < 11; i++) {
        sound_effect[i] = nullptr;
    }
    eat_dot_time = 0;
    old_move_type = -1;
    new_move_type = MOVE_0;
    ghost_turn_frighten = false;
    ghost_go_home = false;
    dead = false;
}

SoundManager :: ~SoundManager()
{
    for(int i = 0; i < 11; i++) {
        Mix_FreeChunk(sound_effect[i]);
        sound_effect[i] = nullptr;
    }
}

void SoundManager :: insert_playlist(const int sound_id)
{
    if(sound_id == EAT_DOT) {
        eat_dot_time = 16;
    }
    else if(sound_id == EAT_GHOST) {
        Mix_PlayChannel(4 , sound_effect[EAT_GHOST] , 0);
    }
    else if(sound_id == GHOST_GO_HOME) {
        ghost_go_home = true;
    }
    else if(sound_id == REVIVAL_GHOST) {
        ghost_go_home = false;
    }
    else if(sound_id == GHOST_TURN_FRIGHTEN) {
        ghost_turn_frighten = true;
    }
    else if(sound_id == NORMAL_GHOST) {
        ghost_turn_frighten = false;
    }
    else if(sound_id == MOVE_0 || sound_id == MOVE_1 || sound_id == MOVE_2 || sound_id == MOVE_3) {
        new_move_type = sound_id;
    }
    else if(sound_id == DEAD) {
        dead = true;
        eat_dot_time = 0;           Mix_Pause(3);
        ghost_turn_frighten = 0;    Mix_Pause(5);
        ghost_go_home = 0;          Mix_Pause(6);
        Mix_Pause(1);
    }
    else if(sound_id == START || sound_id == WINNING) {
        dead = false;
        eat_dot_time = 0;           Mix_Pause(3);
        ghost_turn_frighten = 0;    Mix_Pause(5);
        ghost_go_home = 0;          Mix_Pause(6);
        Mix_Pause(1);
        if(sound_id == WINNING) {
            old_move_type = MOVE_0;
        }
        else {
            old_move_type = -1;
        }
        Mix_PlayChannel(2 , sound_effect[sound_id] , 0);
    }
}

void SoundManager :: load_sound()
{
    sound_effect[MOVE_0]          = Mix_LoadWAV("Assets/Sound/move 0.wav");
    sound_effect[MOVE_1]          = Mix_LoadWAV("Assets/Sound/move 1.wav");
    sound_effect[MOVE_2]          = Mix_LoadWAV("Assets/Sound/move 2.wav");
    sound_effect[MOVE_3]          = Mix_LoadWAV("Assets/Sound/move 3.wav");
    sound_effect[START]           = Mix_LoadWAV("Assets/Sound/start.wav");
    sound_effect[DEAD]            = Mix_LoadWAV("Assets/Sound/dead2.wav");
    sound_effect[WINNING]         = Mix_LoadWAV("Assets/Sound/next level.wav");
    sound_effect[EAT_DOT]         = Mix_LoadWAV("Assets/Sound/eat dot.wav");
    sound_effect[EAT_GHOST]       = Mix_LoadWAV("Assets/Sound/eat ghost.wav");
    sound_effect[GHOST_GO_HOME]   = Mix_LoadWAV("Assets/Sound/ghost go home.wav");
    sound_effect[GHOST_TURN_FRIGHTEN] = Mix_LoadWAV("Assets/Sound/ghost turn blue.wav");

    for(int i = 0; i < 11; i++) {
        if(sound_effect == nullptr) {
            Console -> status(Mix_GetError());
        }
    }

    Mix_PlayChannel(1 , sound_effect[MOVE_0] , -1);                 Mix_Pause(1);
    Mix_PlayChannel(3 , sound_effect[EAT_DOT] , -1);                Mix_Pause(3);
    Mix_PlayChannel(5 , sound_effect[GHOST_TURN_FRIGHTEN] , -1);    Mix_Pause(5);
    Mix_PlayChannel(6 , sound_effect[GHOST_GO_HOME] , -1);          Mix_Pause(6);
    Mix_Pause(8);

    /*
        channel 1: MOVE_0 , MOVE_1 , MOVE_2 , MOVE_3
        channel 2: START / DEAD
        channel 3: EAT_DOT
        channel 4: EAT_GHOST
        channel 5: GHOST_TURN_FRIGHTEN
        channel 6: GHOST_GO_GOME
        channel 7:
        channel 8:
    */
}

void SoundManager :: play_sound()
{
    if(dead) {
        Mix_PlayChannel(2 , sound_effect[DEAD] , 0);
        dead = false;
    }
    if(Mix_Playing(2)) return;

    if(new_move_type != old_move_type){
        Mix_PlayChannel(1, sound_effect[new_move_type], -1);
        old_move_type = new_move_type;
    }

    if(eat_dot_time > 0) {
        eat_dot_time--;
        Mix_Resume(3);
    }
    else Mix_Pause(3);

    if(ghost_turn_frighten) Mix_Resume(5);
    else Mix_Pause(5);

    if(ghost_go_home){
        Mix_Resume(6);
        if(ghost_turn_frighten) Mix_Pause(5);
    }
    else{
        Mix_Pause(6);
        if(ghost_turn_frighten) Mix_Resume(5);
    }
}

void SoundManager :: reset_sound()
{
    Mix_PlayChannel(1 , sound_effect[old_move_type] , -1);
}
