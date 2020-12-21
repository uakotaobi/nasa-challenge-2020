#include "button_view.h"
#include "asset_manager.h"
#include "SDL.h"

ButtonView::ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor, std::function<void()> callback) {
    buttonBoundary_ = buttonBoundary;
    buttonText_ = buttonText;
    buttonColor_ = buttonColor;
    borderColor_ = SDL_Color{55, 128, 189};
    callback_ = callback;
    const AssetManager& am = getAssetManager();
    TTF_Font* buttonFont = am.getFont("gidole");
    renderText = TTF_RenderUTF8_Shaded(buttonFont, buttonText.c_str(), SDL_Color{0, 0, 0, 255}, buttonColor_);

};

ButtonView::~ButtonView() {
    SDL_FreeSurface(renderText);
}

SDL_Rect ButtonView::boundary() const {
    return SDL_Rect {
      buttonBoundary_.x,
      buttonBoundary_.y,
      buttonBoundary_.w,
      buttonBoundary_.h
    };
}

void ButtonView::draw(SDL_Surface* screen) {

    SDL_Rect buttonBoundary = buttonBoundary_;

    SDL_Rect innerButtonBoundary = {buttonBoundary.x+5, buttonBoundary.y+5, buttonBoundary.w-10, buttonBoundary.h-10};

    // Draw the border.
    SDL_FillRect(screen, &buttonBoundary, SDL_MapRGB(screen->format, borderColor_.r, borderColor_.g, borderColor_.b));

    // Change color when mouseover.
    if (mouseOver()) {
        SDL_FillRect(screen, &buttonBoundary, SDL_MapRGB(screen->format, 255, 255, 255));
    }

    // Draw the button background.
    SDL_FillRect(screen, &innerButtonBoundary, SDL_MapRGB(screen->format, buttonColor_.r, buttonColor_.g, buttonColor_.b));

    // Draw the button text.
    SDL_Rect rec = SDL_Rect{
      buttonBoundary.x + buttonBoundary.w / 2 - renderText->w / 2 ,
      buttonBoundary.y + buttonBoundary.h / 2 - renderText->h / 2,
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

void ButtonView::changeBoundary(SDL_Rect newBoundary) {
    buttonBoundary_ = newBoundary;
}

