#include <SDL.h>
#include <iostream>

int main(int argc,char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO)!=0) {
        std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
        return 1;
    }

    SDL_Window* window=SDL_CreateWindow("SDL2 Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN);
    if(!window) {
        std::cout<<"SDL_CreateWindow Error: "<<SDL_GetError()<<std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        std::cout<<"SDL_CreateRenderer Error: "<<SDL_GetError()<<std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer,0,0,255,255); // Màu nền xanh dương
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000); // Hiển thị cửa sổ trong 3 giây

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
