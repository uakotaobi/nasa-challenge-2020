#include "menu_view.h"

MenuView::MenuView(SDL_Surface* surf, bool& done)
    : startButton (ButtonView(SDL_Rect{(surf->w-150)/2, (surf->h-75)/2 - 100, 150, 75},
                             "Start",
                             SDL_Color{112, 191, 255},
                             [] () {})),
      quitButton (ButtonView(SDL_Rect{(surf->w-150)/2, (surf->h-75)/2 + 100, 150, 75},
                            "Quit",
                            SDL_Color{112, 191, 255},
                            [&done] () {
                                done = true;
                            })) {}

void MenuView::draw(SDL_Surface* screen) {
    // Draw HEROIC background image


    // Draw the buttons
    startButton.draw(screen);
    quitButton.draw(screen);
}

SDL_Rect MenuView::boundary() const {
    return SDL_Rect{0, 0, 0, 0};
}

void MenuView::handleClicks(SDL_MouseButtonEvent& mouseButtonEvent) {
    if (startButton.mouseOver()) {
        startButton.handleClicks(mouseButtonEvent);
    } else if (quitButton.mouseOver()) {
        quitButton.handleClicks(mouseButtonEvent);
    }
}

void MenuView::handleResize(SDL_Surface* newSurface) {
  startButton.handleResize(newSurface);
  quitButton.handleResize(newSurface);
}
