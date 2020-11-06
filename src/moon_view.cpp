#include "moon_view.h"
#include "asset_manager.h"
#include "SDL.h"

MoonView::MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY) {
    boundaryMoonView = moonBoundary;
}

void MoonView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &boundaryMoonView, SDL_MapRGB(screen->format, 164, 166, 166));
}

SDL_Rect MoonView::boundary() const {
    return boundaryMoonView;
}
