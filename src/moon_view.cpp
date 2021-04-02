#include "moon_view.h"
#include "asset_manager.h"
#include "SDL.h"

MoonView::MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY) {
    boundaryMoonView = moonBoundary;
    moonGrid = Grid(300, 300, 6.0);
    camera = Basis();
}

void MoonView::draw(SDL_Surface* screen) {
    // Viewport is filled with black.
    SDL_FillRect(screen, &boundaryMoonView, SDL_MapRGB(screen->format, 0, 0, 0));

    // This function no longer exists.
    // moonGrid.render(screen, boundaryMoonView, camera);
}

SDL_Rect MoonView::boundary() const {
    return boundaryMoonView;
}

void MoonView::setBoundary(SDL_Rect newBoundary) {
    boundaryMoonView = newBoundary;
}

void MoonView::setCamera(const Basis& newCamera) {
    camera = newCamera;
}

Grid& MoonView::getGrid() {
    return moonGrid;
}

void MoonView::drawWithRenderer(const Renderer& r) const {
    // Viewport is filled with black.
    SDL_FillRect(r.getScreen(), &boundaryMoonView, SDL_MapRGB(r.getScreen()->format, 0, 0, 0));

    moonGrid.render(r);
}
