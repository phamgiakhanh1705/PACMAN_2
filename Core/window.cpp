#include "window.h"
#include "../LogStatus/logstatus.h"

// Khởi tạo các con trỏ và trạng thái ban đầu
Window :: Window()
{
    window = nullptr;
    renderer = nullptr;
    play_state = nullptr;
    start_menu = nullptr;
    menu_running = false;
}

// Giải phóng bộ nhớ và hủy renderer/window nếu tồn tại
Window :: ~Window()
{
    delete play_state;
    play_state = nullptr;

    delete start_menu;
    start_menu = nullptr;

    if(renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if(window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}


// Khởi tạo SDL
void Window :: init_SDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Console -> status(SDL_GetError());
    }
    else {
        // Tạo cửa sổ game: tiểu đề , vị trí , kích thước , chế độ hiển thị
        window = SDL_CreateWindow(WINDOW_TITLE.c_str() , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT , SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        Console -> status("Window opened successfully!");
        running = true;

        if(window == nullptr) { // Tạo cửa sổ lỗi
            Console -> status(SDL_GetError());
        }
        else {
            // Tạo renderer để vẽ lên window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            Console -> status("Renderer created successfully!");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // Giúp khi scale hình ảnh thì làm mượt
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT); // Dù resize cửa sổ, vẫn giữ game theo tỉ lệ 960x540, tránh vỡ layout

            if(TTF_Init() < 0) Console -> status(TTF_GetError());
            else Console -> status("TTF Ready!");

            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) Console -> status(Mix_GetError());
            else Console -> status("Audio Ready!");
            Mix_Volume(-1, MIX_MAX_VOLUME); // Đặt âm lượng mặc định cho tất cả channel max volume

            freopen("score.txt" , "r" , stdin);
            std :: string text = "";
            for(int i = 0; i < 10; i++){
                std :: getline(std :: cin , text);
                highscores.push_back(text);
            }
        }
    }
}

void Window :: quit_SDL()
{
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    TTF_Quit();
    Mix_Quit();

    atexit(SDL_Quit);
    Console -> status("Successfully exited!");
    freopen("score.txt" , "w" , stdout);
    for(int i = 0; i < 10; i++) std :: cout << highscores[i] << '\n';
    return;
}

void Window :: start_running_game()
{
    start_menu = new Menu(262 , 170 , (int) start_menu_button_text.size() , 320 , 30);
    start_menu -> init(renderer , "Assets/Menu Image/Pacman Pause Menu.png" , start_menu_button_text);
    start_menu -> change_running_status();

    SDL_Event event;
    menu_running = true;
    bool start_game = false;
    play_state = new PlayStateManager();

    while(running) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) running = false;
            else {
                if(menu_running) {
                    start_menu -> handle_event(event , renderer);
                    if(start_menu -> get_menu_status() == Menu :: PLAY_BUTTON_PRESSED) {
                        menu_running = false;
                    }
                    if(start_menu -> get_menu_status() == Menu :: EXIT_BUTTON_PRESSED) {
                        running = false;
                    }
                }
                else {
                    play_state -> handle_event(event , renderer , menu_running , highscores);
                    if(menu_running) start_menu -> return_main_menu();
                }
            }
        }
        if(!menu_running) {
            if(!start_game) {
                play_state -> new_game(renderer);
                start_game = true;
            }
            play_state -> run_game(menu_running);
            if(menu_running) {
                start_menu -> return_main_menu();
                start_game = false;
            }
        }

        SDL_SetRenderDrawColor(renderer , 0 , 0 , 0 , 255);
        SDL_RenderClear(renderer);

        if(menu_running) {
            start_menu -> render_menu(renderer , highscores);
        }
        else play_state -> render(renderer , highscores);

        SDL_RenderPresent(renderer);
    }
}
