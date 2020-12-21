#include "menu_view.h"
#include "asset_manager.h"

const int buttonWidth = 150;
const int buttonHeight = 75;
const int displacement = 100;

MenuView::MenuView(SDL_Surface* surf, int& currentView)
    : startButton (ButtonView(SDL_Rect{(surf->w-buttonWidth)/2, (surf->h-buttonHeight)/2 - displacement, buttonWidth, buttonHeight},
                             "Start",
                             SDL_Color{112, 191, 255},
                             [&currentView] () {currentView = 1;})),
      quitButton (ButtonView(SDL_Rect{(surf->w-buttonWidth)/2, (surf->h-buttonHeight)/2 + displacement, buttonWidth, buttonHeight},
                            "Quit",
                            SDL_Color{112, 191, 255},
                            [&currentView] () {currentView = -1;})),
      heroicImage(nullptr) {
      const AssetManager& am = getAssetManager();
      heroicImage = am.getImage("Apollo_15_flag,_rover,_LM,_Irwin.jpg");
}

MenuView::~MenuView() {
      if (heroicImage) {
          SDL_FreeSurface(heroicImage);
      }
}

void MenuView::draw(SDL_Surface* screen) {
    // Draw HEROIC background image
    float aspectRatio = heroicImage->w / heroicImage->h;
    SDL_Rect blitDestinationRect = SDL_Rect{0, 0, screen->w, int(screen->w/aspectRatio)};
    blitDestinationRect.x = screen->w / 2 - blitDestinationRect.w / 2;
    blitDestinationRect.y = screen->h / 2 - blitDestinationRect.h / 2;
    SDL_BlitScaled(heroicImage, nullptr, screen, &blitDestinationRect);

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
    startButton.changeBoundary(SDL_Rect{(newSurface->w-buttonWidth)/2, (newSurface->h-buttonHeight)/2 - displacement, buttonWidth, buttonHeight});
    quitButton.changeBoundary(SDL_Rect{(newSurface->w-buttonWidth)/2, (newSurface->h-buttonHeight)/2 + displacement, buttonWidth, buttonHeight});
}
