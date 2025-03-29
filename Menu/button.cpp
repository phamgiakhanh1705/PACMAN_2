#include "Button.h"

Menu_button :: Menu_button(int width, int height, int pos_x, int pos_y)
{
    button_rect = {pos_x, pos_y, width, height};
    default_label = new Text_manager(MAIN_LABEL_FONT_SIZE);
    highlighted_label = new Text_manager(MAIN_LABEL_FONT_SIZE);
    detail_label = new Text_manager(DETAIL_LABEL_FONT_SIZE);
}

void Menu_button :: load_label(SDL_Renderer* &renderer, std :: string text)
{
    if(text == "") return;
    default_label -> load_render_text(renderer, text, default_color);
    highlighted_label -> load_render_text(renderer, text, select_color);
    label_text = text;

    if(text == "")
    {
        detail_text = "";
    }
    else
    {
        std :: string lower_text = text;
        for(char &c : lower_text) c = std :: tolower(c);

        if(lower_text == "new game") detail_text = "Press Enter to play a new game.";
        else if(lower_text == "resume") detail_text = "Press Enter to continue the game.";
        else if(lower_text == "how to play") detail_text = "Press Enter for instructions on how to play.";
        else if(lower_text == "high scores") detail_text = "Press Enter for High score.";
        else if(lower_text == "sound: on") detail_text = "Press Enter to change sound status.";
        else if(lower_text == "exit") detail_text = "Press Enter to quit the game.";
        else if(lower_text == "exit to start menu") detail_text = "Press Enter to return to the Start Menu.";
        else detail_text = "";
    }

    if(detail_text != "")
    {
        detail_label -> load_render_text(renderer, detail_text, default_color);
    }
    else
    {
        delete detail_label;
        detail_label = nullptr;
    }
}

void Menu_button :: button_render(SDL_Renderer* &renderer)
{
    if(state == STATE_HOVERED)
    {
        SDL_SetRenderDrawColor(renderer, 251, 69, 152, 255);
        SDL_RenderFillRect(renderer, &button_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &button_rect);
        highlighted_label -> render_text(renderer, button_rect.x + button_rect.w / 2, button_rect.y + button_rect.h / 2, Text_manager :: CENTER);
        if(detail_label != nullptr)
            detail_label -> render_text(renderer, button_rect.x + button_rect.w / 2, button_rect.y + button_rect.h + 20, Text_manager :: CENTER);
    }
    else
    {
        default_label -> render_text(renderer, button_rect.x + button_rect.w / 2, button_rect.y + button_rect.h / 2, Text_manager :: CENTER);
    }
}

void Menu_button :: set_state(const int new_state)
{
    state = new_state;
}

void Menu_button :: toggle_sound_label(SDL_Renderer* &renderer)
{
    if(label_text == "Sound: ON")
    {
        load_label(renderer, "Sound: OFF");
    }
    else
    {
        load_label(renderer, "Sound: ON");
    }
}

bool Menu_button :: is_mouse_over(const int &x, const int &y) const
{
    return (x >= button_rect.x && x <= button_rect.x + button_rect.w && y >= button_rect.y && y <= button_rect.y + button_rect.h);
}

std :: string Menu_button :: get_label() const
{
    return label_text;
}
