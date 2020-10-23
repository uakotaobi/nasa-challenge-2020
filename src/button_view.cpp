#include "button_view.h"

ButtonView::ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor) {
    buttonBoundary_ = buttonBoundary;
    buttonText_ = buttonText;
    buttonColor_ = buttonColor;

};
ButtonView::~ButtonView() {

}

SDL_Rect ButtonView::boundary() const {
    return buttonBoundary_;
}

void ButtonView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &buttonBoundary_, SDL_MapRGB(screen->format, buttonColor_.r, buttonColor_.g, buttonColor_.b));

}
