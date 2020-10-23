#include "button_view.h"
#include "font_manager.h"

ButtonView::ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor) {
    buttonBoundary_ = buttonBoundary;
    buttonText_ = buttonText;
    buttonColor_ = buttonColor;
    borderColor_ = SDL_Color{232, 145, 51};

};
ButtonView::~ButtonView() {

}

SDL_Rect ButtonView::boundary() const {
    return buttonBoundary_;
}

void ButtonView::draw(SDL_Surface* screen) {
    SDL_Rect innerButtonBoundary = {buttonBoundary_.x+5, buttonBoundary_.y+5, buttonBoundary_.w-10, buttonBoundary_.h-10};
    SDL_FillRect(screen, &buttonBoundary_, SDL_MapRGB(screen->format, borderColor_.r, borderColor_.g, borderColor_.b));
    SDL_FillRect(screen, &innerButtonBoundary, SDL_MapRGB(screen->format, buttonColor_.r, buttonColor_.g, buttonColor_.b));

}
