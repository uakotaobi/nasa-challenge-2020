#include "nav_view.h"
#include "asset_manager.h"
#include "SDL.h"

NavView::NavView(SDL_Rect navBoundary, int deltaX, int deltaY) {
    boundaryNavView = navBoundary;
}

void NavView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &boundaryNavView, SDL_MapRGB(screen->format, 15, 181, 0));
}

SDL_Rect NavView::boundary() const {
    return boundaryNavView;
}
