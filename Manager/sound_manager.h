#pragma once

#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include <SDL_mixer.h>
#include <vector>
#include "../LogStatus/logstatus.h"

class SoundManager
{
    private:

        bool dead;
        bool ghost_turn_frighten;
        bool ghost_go_home;
        int eat_dot_time;
        int old_move_type , new_move_type;
        log_status* Console = new log_status("Sound Manager");
        Mix_Chunk* sound_effect[11];

    public:

        static const int MOVE_0 = 0;
        static const int MOVE_1 = 1;
        static const int MOVE_2 = 2;
        static const int MOVE_3 = 3;
        static const int START = 4;
        static const int EAT_DOT = 5;
        static const int EAT_GHOST = 6;
        static const int GHOST_TURN_FRIGHTEN = 7;
        static const int GHOST_GO_HOME = 8;
        static const int DEAD = 9;
        static const int WINNING = 10;
        static const int TOTAL_SOUND_EFFECT = 11;
        static const int NORMAL_GHOST = 11;
        static const int REVIVAL_GHOST = 12;

        SoundManager();
        ~SoundManager();

        void insert_playlist(const int sound_id);

        void load_sound();

        void play_sound();

        void reset_sound();
};

#endif // _SOUNDMANAGER_H_
