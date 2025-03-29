#pragma once

#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <SDL_mixer.h>
#include "../LogStatus/logstatus.h"
#include <iostream>

class Sound_manager
{
    public:

        static const int step_0 = 0;
        static const int step_1 = 1;
        static const int step_2 = 2;
        static const int step_3 = 3;
        static const int start = 4;
        static const int chomp = 5;
        static const int ghost_eaten = 6;
        static const int ghost_turn_blue = 7;
        static const int ghost_home = 8;
        static const int pacman_die = 9;
        static const int win_level = 10;
        static const int normal_ghost = 11;
        static const int revive_ghost = 12;
        static const int total_sounds = 11;

        Sound_manager();
        ~Sound_manager();

        void play_sound();
        void load_sound();
        void queue_sound(int sound_id);
        void clear_status();

    private:

        bool is_dead;
        bool ghost_frightened;
        bool ghost_escaping;
        int dot_timer;
        int prev_move_sound;
        int curr_move_sound;

        log_status* logger;
        Mix_Chunk* sounds[13];

};

#endif //_SOUND_MANAGER_H_
