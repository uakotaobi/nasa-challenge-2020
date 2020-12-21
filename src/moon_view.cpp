#include "moon_view.h"
#include "asset_manager.h"
#include "SDL.h"

MoonView::MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY) {
    boundaryMoonView = moonBoundary;
    moonGrid = Grid(300, 300, 6.0);

    // Move the camera so it's above the grid and tilted down towards the grid.
    camera = Basis();
    Matrix translationMatrix = ::translationMatrix(Vector(0, 400, 0));
    Matrix rotationMatrix = ::rotationMatrix(camera.center, camera.center + camera.axisX, 75);
    camera.apply(rotationMatrix * translationMatrix);
}

void MoonView::draw(SDL_Surface* screen) {
    // Viewport is filled with black.
    SDL_FillRect(screen, &boundaryMoonView, SDL_MapRGB(screen->format, 0, 0, 0));

    moonGrid.render(screen, boundaryMoonView, camera);
}

SDL_Rect MoonView::boundary() const {
    return boundaryMoonView;
}
