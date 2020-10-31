#include "button_view.h"
#include "font_manager.h"
#include "SDL.h"

ButtonView::ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor, std::function<void()> callback) {
    buttonBoundary_ = buttonBoundary;
    buttonText_ = buttonText;
    buttonColor_ = buttonColor;
    borderColor_ = SDL_Color{55, 128, 189};
    callback_ = callback;
    deltaX = 0;
    deltaY = 0;
    const FontManager& fm = getFontManager();
    TTF_Font* buttonFont = fm.getFont("gidole");
    renderText = TTF_RenderUTF8_Shaded(buttonFont, buttonText.c_str(), SDL_Color{0, 0, 0, 255}, buttonColor_);

};

ButtonView::~ButtonView() {
    SDL_FreeSurface(renderText);
}

SDL_Rect ButtonView::boundary() const {
    return buttonBoundary_;
}

void ButtonView::draw(SDL_Surface* screen) {

    // Remember displacement of this button from center of surface.
    deltaX = buttonBoundary_.x - screen->w / 2;
    deltaY = buttonBoundary_.y - screen->h / 2;

    SDL_Rect innerButtonBoundary = {buttonBoundary_.x+5, buttonBoundary_.y+5, buttonBoundary_.w-10, buttonBoundary_.h-10};

    // Draw the border.
    SDL_FillRect(screen, &buttonBoundary_, SDL_MapRGB(screen->format, borderColor_.r, borderColor_.g, borderColor_.b));

    // Change color when mouseover.
    if (mouseOver()) {
        SDL_FillRect(screen, &buttonBoundary_, SDL_MapRGB(screen->format, 255, 255, 255));
    }

    // Draw the button background.
    SDL_FillRect(screen, &innerButtonBoundary, SDL_MapRGB(screen->format, buttonColor_.r, buttonColor_.g, buttonColor_.b));

    // Draw the button text.
    SDL_Rect rec = SDL_Rect{
      buttonBoundary_.x + buttonBoundary_.w / 2 - renderText->w / 2 ,
      buttonBoundary_.y + buttonBoundary_.h / 2 - renderText->h / 2,
      0 /* ignored */,
      0 /* ignored */
    };
    SDL_BlitSurface(renderText,
                    nullptr,
                    screen,
                    &rec);
}

void ButtonView::handleClicks(SDL_MouseButtonEvent& mouseButtonEvent) {
    callback_();
}


void ButtonView::handleResize(SDL_Surface* newSurface) {
    buttonBoundary_.x = newSurface->w / 2 + deltaX;
    buttonBoundary_.y = newSurface->h / 2 + deltaY;
}
