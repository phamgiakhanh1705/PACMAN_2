#pragma once

#ifndef _TICK_MANAGER_H_
#define _TICK_MANAGER_H_

#include <SDL.h>
#include <vector>

using mode_entry_t = std :: pair<const int , double>;

class Tick_manager
{
    private:

        unsigned int last_tick;
        unsigned int frightened_tick;
        unsigned int greendy_tick;
        unsigned int last_frame = 0;

        std :: vector <mode_entry_t> mode_timeline;

        int last_status;
        bool is_paused;

        double frightened_duration = 5.0;
        double scatter_duration = 7.0;
        double chase_duration = 20.0;

    public:

        static const int FPS = 60;
        static constexpr double FRIENDY_CHASE_DURATION = 3.0;
        static constexpr double GREENDY_CHASE_DURATION = 2.0;
        static const int MODE_FRIGHTENED = 0;
        static const int MODE_SCATTER = 1;
        static const int MODE_CHASE = 2;
        static constexpr double TIME_INFINITY = -1.0;

        Tick_manager();

        double get_remaining_frightened_time() const;

        bool is_frightened_time() const;

        bool is_scatter_time() const;

        bool is_friendy_chase_time() const;

        bool is_greendy_chase_time() const;

        void start_friendy_chase();

        void start_greendy_chase();

        void activate_frightened_mode();

        void reset_tick(const int level);

        void update_mode_status();

        void stabilize_fps();

        bool toggle_pause(const bool status);
};

#endif // _TICK_MANAGER_H_
