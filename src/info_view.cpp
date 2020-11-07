#include "info_view.h"
#include "asset_manager.h"
#include "SDL.h"

InfoView::InfoView(SDL_Rect infoBoundary, int deltaX, int deltaY) {
    boundaryInfoView = infoBoundary;
}

void InfoView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &boundaryInfoView, SDL_MapRGB(screen->format, 255, 61, 64));
}

SDL_Rect InfoView::boundary() const {
    return boundaryInfoView;
}
