#include "menu_view.h"
#include "asset_manager.h"


MenuView::MenuView(SDL_Surface* surf, int& currentView)
    : startButton (ButtonView(SDL_Rect{(surf->w-150)/2, (surf->h-75)/2, 150, 75},
                             "Start",
                             SDL_Color{112, 191, 255},
                             [&currentView] () {currentView = 1;},
                             -75, -100)),
      quitButton (ButtonView(SDL_Rect{(surf->w-150)/2, (surf->h-75)/2, 150, 75},
                            "Quit",
                            SDL_Color{112, 191, 255},
                            [&currentView] () {currentView = -1;},
                            -75, 100)),
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
  startButton.handleResize(newSurface);
  quitButton.handleResize(newSurface);
}
