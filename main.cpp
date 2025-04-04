#include "Core/Window.h"

int main(int argc, char* argv[])
{

    Window* window = new Window();

    window -> init_SDL();

    window -> start_running_game();

    window -> quit_SDL();
    return 0;
}
