#include "Menu.h"
#include <stdio.h>

Menu :: Menu(const int _base_screen_pos_col , const int _base_screen_pos_row , const int total_button , const int button_width , const int button_height)
{
    menu_texture = nullptr;
    for(int i = 0; i < 3; i++) {
        how_to_play_texture[i] = nullptr;
    }
    high_score_texture = nullptr;
    arrow_texture = nullptr;
    score_text = nullptr;
    arrow_HS_pos = 0;
    TOTAL_BUTTON = total_button;
    MENU_BUTTON_WIDTH = button_width;
    MENU_BUTTON_HEIGHT = button_height;
    base_screen_pos_col = _base_screen_pos_col;
    base_screen_pos_row = _base_screen_pos_row;
}

Menu :: ~Menu()
{
    Mix_FreeChunk(navigation_sound);
    Mix_FreeChunk(selection_sound);

    SDL_DestroyTexture(menu_texture);
    menu_texture = nullptr;

    for(int i = 0; i < 3; i++) {
        SDL_DestroyTexture(how_to_play_texture[i]);
        how_to_play_texture[i] = nullptr;
    }

    SDL_DestroyTexture(high_score_texture);
    high_score_texture = nullptr;

    SDL_DestroyTexture(arrow_texture);
    arrow_texture = nullptr;

    for(Button* button : menu_button){
        delete button;
        button = nullptr;
    }
    menu_button.clear();
}

void Menu :: render_menu(SDL_Renderer* &renderer , const std :: vector <std :: string> &score_data)
{
    if(current_menu_status == HOW_TO_PLAY) {
        SDL_RenderCopy(renderer , how_to_play_texture[current_HTP_id] , nullptr , nullptr);
    }
    else if(current_menu_status == HIGH_SCORES) {
        SDL_RenderCopy(renderer , high_score_texture , nullptr , nullptr);

        SDL_Rect source_UP = {0 , 0 , 431 , 400} , source_DOWN = {431 , 0 , 431 , 400};
        SDL_Rect display_UP = {441 - 41 , 85 , 80 , 80} , display_DOWN = {441 - 34 , 385 , 80 , 80};

        static bool alpha_up = true;
        if(alpha_up) alpha_mod += 2;
        else alpha_mod -= 2;
        if(alpha_mod > 255) alpha_up = false , alpha_mod = 254;
        else if(alpha_mod < 1) alpha_up = true , alpha_mod = 0;

        SDL_SetTextureAlphaMod(arrow_texture , alpha_mod);
        SDL_RenderCopy(renderer , arrow_texture , &source_UP , &display_UP);
        SDL_RenderCopy(renderer , arrow_texture , &source_DOWN , &display_DOWN);

        for(int i = arrow_HS_pos; i < arrow_HS_pos + 7; i++) {
            score_text -> load_render_text(renderer, score_data[i] , {0 , 0 , 0 , 255});
            score_text -> render_text(renderer , 441 , 160 + 40 * (i - arrow_HS_pos) , TextManager :: CENTER);
        }
    }
    else {
        SDL_RenderCopy(renderer , menu_texture , nullptr , nullptr);
        for(int i = 0; i < TOTAL_BUTTON; i++) {
            menu_button[i] -> render_button(renderer);
        }
    }
}

void Menu :: init(SDL_Renderer* &renderer , const std :: string image_path ,  std :: vector <std :: string> &button_text)
{
    SDL_Surface* image = IMG_Load(image_path.c_str());
    if(image == nullptr) console -> status(IMG_GetError());
    else {
        menu_texture = SDL_CreateTextureFromSurface(renderer , image);
        SDL_FreeSurface(image);

        how_to_play_texture[0] = load_image(renderer , "Assets/Menu Image/How to Play1.png");
        how_to_play_texture[1] = load_image(renderer , "Assets/Menu Image/How to Play2.png");
        how_to_play_texture[2] = load_image(renderer , "Assets/Menu Image/How to Play3.png");

        image = IMG_Load("Assets/Menu Image/High Scores.png");
        high_score_texture = load_image(renderer , "Assets/Menu Image/High Scores.png");
        arrow_texture = load_image(renderer , "Assets/Menu Image/arrow-upanddown.png");

        score_text = new TextManager(28);

        for(int i = 0; i < TOTAL_BUTTON; i++) {
            menu_button.push_back(new Button(MENU_BUTTON_WIDTH ,
                                             MENU_BUTTON_HEIGHT ,
                                             base_screen_pos_col - MENU_BUTTON_WIDTH / 2 ,
                                             base_screen_pos_row + (5 + MENU_BUTTON_HEIGHT) * (i - 1)));
        }

        for(int i = 0; i < TOTAL_BUTTON; i++) {
            if(button_text[i] == "Sound: ON") {
                if(Mix_Volume(-1 , -1) == 0) button_text[i] = "Sound: OFF";
            }
            menu_button[i] -> load_button(renderer , button_text[i]);
            menu_button[i] -> set_status(Button :: BUTTON_OUT);
        }
        menu_button[0] -> set_status(Button :: BUTTON_IN);
        current_button_id = 0;
        current_menu_status = RUNNING;
    }
}

void Menu :: handle_event(SDL_Event &event, SDL_Renderer* &renderer)
{
    if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
            Mix_PlayChannel(7 , navigation_sound , 0);
            if(current_menu_status == HIGH_SCORES) {
                if(arrow_HS_pos < 3) arrow_HS_pos++;
            }
            else {
                menu_button[current_button_id] -> set_status(Button :: BUTTON_OUT);
                (current_button_id += 1) %= TOTAL_BUTTON;
                menu_button[current_button_id] -> set_status(Button :: BUTTON_IN);
            }
        }
        else if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
            Mix_PlayChannel(7 , navigation_sound , 0);
            if(current_menu_status == HIGH_SCORES) {
                if(arrow_HS_pos > 0) arrow_HS_pos--;
            }
            else {
                menu_button[current_button_id] -> set_status(Button :: BUTTON_OUT);
                (current_button_id += TOTAL_BUTTON - 1) %= TOTAL_BUTTON;
                menu_button[current_button_id] -> set_status(Button :: BUTTON_IN);
            }
        }
        else if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
            Mix_PlayChannel(7 , navigation_sound , 0);
            if(current_menu_status == HOW_TO_PLAY) {
                if(current_HTP_id > 0) current_HTP_id--;
            }
        }
        else if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
            Mix_PlayChannel(7 , navigation_sound , 0);
            if(current_menu_status == HOW_TO_PLAY) {
                if(current_HTP_id < 2) current_HTP_id++;
            }
        }
        else if(event.key.keysym.sym == SDLK_RETURN) {
            Mix_PlayChannel(7 , navigation_sound , 0);
            if(current_menu_status == HOW_TO_PLAY || current_menu_status == HIGH_SCORES) {
                menu_button[current_button_id] -> set_status(Button :: BUTTON_IN);
                current_menu_status = RUNNING;
            }
            else {
                menu_button[current_button_id] -> set_status(Button :: BUTTON_IN);
                std :: string menu_text = menu_button[current_button_id] -> get_text();
                if(menu_text == "New Game") current_menu_status = PLAY_BUTTON_PRESSED;
                else if(menu_text == "Resume") current_menu_status = RESUME;
                else if(menu_text == "Exit") current_menu_status = EXIT_BUTTON_PRESSED;
                else if(menu_text == "Exit to Start Menu") current_menu_status = EXIT_BUTTON_PRESSED;
                else if(menu_text == "Sound: ON") {
                    menu_button[current_button_id] -> change_sound_button(renderer);
                    Mix_Volume(-1 , 0);
                }
                else if(menu_text == "Sound: OFF") {
                    menu_button[current_button_id] -> change_sound_button(renderer);
                    Mix_Volume(-1 , MIX_MAX_VOLUME);
                }
                else if(menu_text == "How to Play") {
                    current_menu_status = HOW_TO_PLAY;
                    current_HTP_id = 0;
                }
                else if(menu_text == "High Scores") {
                    current_menu_status = HIGH_SCORES;
                    arrow_HS_pos = 0;
                }
            }
        }
    }
}

bool Menu :: is_menu_running() const
{
    return current_menu_status == RUNNING;
}

int Menu :: get_menu_status() const
{
    return current_menu_status;
}

void Menu :: return_main_menu()
{
    current_menu_status = RUNNING;
    menu_button[current_button_id] -> set_status(Button :: BUTTON_IN);
}

void Menu :: change_running_status()
{
    running = !running;
}

SDL_Texture* Menu :: load_image(SDL_Renderer* &renderer , const std :: string image_path)
{
    SDL_Surface* image = IMG_Load(image_path.c_str());
    SDL_Texture* loadTexture = SDL_CreateTextureFromSurface(renderer , image);
    SDL_FreeSurface(image);
    return loadTexture;
}
