#include "Button.h"

Button :: Button(int width , int height , int screen_pos_col , int screen_pos_row)
{
    button_rect = {screen_pos_col , screen_pos_row , width , height};
    normal_text = new TextManager(24);
    select_text = new TextManager(24);
    select_text_detail = new TextManager(18);
}

void Button :: set_status(const int status)
{
    button_status = status;
}

void Button :: change_sound_button(SDL_Renderer* &renderer)
{
    if(button_text == "Sound: ON") button_text = "Sound: OFF";
    else button_text = "Sound: ON";
    normal_text -> loadRenderText(renderer , button_text , normal_color);
    select_text -> loadRenderText(renderer , button_text , select_color);
    button_status = BUTTON_IN;
}

void Button :: render_button(SDL_Renderer* &renderer)
{
    if(button_status == BUTTON_IN) {
        SDL_SetRenderDrawColor(renderer , 251 , 69 , 152 , 255);
        SDL_RenderFillRect(renderer , &button_rect);

        SDL_SetRenderDrawColor(renderer , select_color.r , select_color.g , select_color.b , select_color.a);
        select_text -> renderText(renderer , button_rect.x + button_rect.w / 2 , button_rect.y + button_rect.h / 2 , TextManager :: CENTER);

        if(select_text_detail != nullptr) {
            SDL_SetRenderDrawColor(renderer , normal_color.r , normal_color.g , normal_color.b , normal_color.a);
            select_text_detail -> renderText(renderer , 441 , 400 , TextManager :: CENTER);
        }
    }
    else if(button_status == BUTTON_OUT) {
        SDL_SetRenderDrawColor(renderer , normal_color.r , normal_color.g , normal_color.b , normal_color.a);
        normal_text -> renderText(renderer , button_rect.x + button_rect.w / 2 , button_rect.y + button_rect.h / 2 , TextManager :: CENTER);
    }
}

void Button :: load_button(SDL_Renderer* &renderer , std :: string text)
{
    if(text == "") return;
    normal_text -> loadRenderText(renderer , text , normal_color);
    select_text -> loadRenderText(renderer , text , select_color);

    button_text = text;
    button_detail = "";

    if (button_text == "New Game") {
        button_detail = "Press Enter to play a new game.";
    }
    if (button_text == "Resume") {
        button_detail = "Press Enter to continue the game.";
    }
    if (button_text == "How to Play") {
        button_detail = "Press Enter for instructions on how to play.";
    }
    if (button_text == "High Scores") {
        button_detail = "Press Enter for High score.";
    }
    if (button_text == "Sound: ON") {
        button_detail = "Press Enter to change sound status.";
    }
    if (button_text == "Exit") {
        button_detail = "Press Enter to quit the game.";
    }
    if (button_text == "Exit to Start Menu") {
        button_detail = "Press Enter to return to the Start Menu.";
    }

    if(button_detail != "") {
        select_text_detail -> loadRenderText(renderer , button_detail , normal_color);
    }
    else {
        delete select_text_detail;
        select_text_detail = nullptr;
    }
}

bool Button :: check_mouse_pos(const int &col, const int &row) const
{
    if(col < button_rect.x || col > button_rect.x + button_rect.w) return false;
    if(row < button_rect.y || row > button_rect.y + button_rect.h) return false;
    return true;
}

std :: string Button :: get_text() const
{
    return button_text;
}
