#pragma once

#ifndef _TICKMANAGER_H_
#define _TICKMANAGER_H_

#include <SDL.h>
#include <stack>

typedef std :: pair <const int , double> CID;

class TickManager
{
    private:

        Uint32 last_tick;
        Uint32 flash_tick;
        Uint32 glast_tick;
        Uint32 last_frame = 0;

        std :: stack <CID> current_mode;

        int last_status;
        bool pause;
        double FRIGHTEN_TIME = 5.0;
        double SCATTER_TIME = 7.0;
        double CHASING_TIME = 20.0;

    public:

        const int FPS = 60;
        const int FRIGHTEN_MODE = 0;
        const int SCATTER_MODE = 1;
        const int CHASING_MODE = 2;
        const double inf = -1;

        TickManager();
        ~TickManager() {};

        double remain_frighten_time() const;

        bool is_frighten_time() const;

        bool is_scatter_time() const;

        void set_frighten_time();

        void reset_tick(const int current_level);

        void update_status();

        void stablize_FPS();

        bool pause_tick(const bool status);
};

#endif // TIMEMANAGER_H_
