#include "window.h"
#include "../LogStatus/logstatus.h"

// Khởi tạo các con trỏ và trạng thái ban đầu
Window :: Window()
{
    window = nullptr;
    renderer = nullptr;
    playState = nullptr;
    startMenu = nullptr;
    runningMenu = false;
}

// Giải phóng bộ nhớ và hủy renderer/window nếu tồn tại
Window :: ~Window()
{
    delete playState;
    playState = nullptr;

    delete startMenu;
    startMenu = nullptr;

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
void Window :: initSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Console -> status(SDL_GetError());
    }
    else{
        // Tạo cửa sổ game: tiểu đề , vị trí , kích thước , chế độ hiển thị
        window = SDL_CreateWindow(WINDOW_TITTLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        Console -> status("Window opened successfully!");
        Running = true;

        if(window == nullptr) { // Tạp cửa sổ lỗi
            Console -> status(SDL_GetError());
        }
        else{
            // Tạo renderer để vẽ lên window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            Console -> status("Renderer created successfully!");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            if(TTF_Init() < 0) Console -> status(TTF_GetError());
            else Console -> status("TTF Ready!");

            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) Console -> status(Mix_GetError());
            else Console -> status("Audio Ready!");
            Mix_Volume(-1, MIX_MAX_VOLUME);

            freopen("score.txt", "r", stdin);
            std :: string t = "";
            for(int i = 0;i < 10;++i){
                std :: getline(std :: cin, t);
                highScore.push_back(t);
            }
        }
    }
}

void Window :: quitSDL()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    Mix_Quit();
    TTF_Quit();
    atexit(SDL_Quit);
    Console -> status("Successfully exited!");
    freopen("score.txt", "w", stdout);
    for(int i = 0;i < 10;++i) std :: cout << highScore[i] << std :: endl;
    return;
}

void Window :: runGame()
{
    startMenu = new Menu(262 , 170 , startMenuButtonText.size(), 320 , 30);
    startMenu -> init(renderer, "Assets/Menu Image/Pacman Pause Menu.png", startMenuButtonText);
    startMenu -> change_running_status();

    SDL_Event e;
    runningMenu = true;
    bool startGame = false;
    playState = new PlayStateManager();

    while(Running){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT) Running = false;
            else{
                if(runningMenu){
                    startMenu -> handle_event(e, renderer);
                    switch(startMenu -> get_menu_status()){
                        case Menu :: PLAY_BUTTON_PRESSED:
                            runningMenu = false;
                            break;
                        case Menu :: EXIT_BUTTON_PRESSED:
                            Running = false;
                            break;
                    }
                }
                else{
                    playState -> handleEvent(e, renderer, runningMenu, highScore);
                    if(runningMenu) startMenu -> return_main_menu();
                }
            }
        }
        if(!runningMenu){
            if(!startGame){
                playState -> newGame(renderer);
                startGame = true;
            }
            playState -> runGame(runningMenu);
            if(runningMenu) startMenu -> return_main_menu(), startGame = false;
        }

        SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255);
        SDL_RenderClear(renderer);

        if(runningMenu)
            startMenu -> render_menu(renderer, highScore);
        else playState -> render(renderer, highScore);

        SDL_RenderPresent(renderer);
    }
}
