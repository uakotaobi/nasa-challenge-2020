#include "main_view.h"
#include "asset_manager.h"

MainView::MainView(SDL_Surface* screen)
    : moonView (MoonView(SDL_Rect{(screen->w/10),(screen->h/20),(screen->w*2/3),(screen->h*4/5)}, 0, 0)),
      infoView (InfoView(SDL_Rect{(screen->w/10*8),(screen->h/20),(screen->w*1/6),(screen->h*4/5)}, 0, 0)),
	  navView (NavView(SDL_Rect{10,(screen->h/20),(screen->w*1/12),(screen->h*4/5)}, 0, 0)) {
    boundaryMainView = SDL_Rect{0, 0, screen->w, screen->h};

    // Move the camera so it's above the grid and tilted down towards the grid.
    camera = Basis();
    Matrix translationMatrix = ::translationMatrix(Vector(0, 400, 0));
    Matrix rotationMatrix = ::rotationMatrix(camera.center, camera.center + camera.axisX, 75);
    camera.apply(rotationMatrix * translationMatrix);
    this->setCamera(camera);
}

void MainView::draw(SDL_Surface* screen) {
    SDL_FillRect(screen, &boundaryMainView, SDL_MapRGB(screen->format, 0, 0, 0));
    moonView.draw(screen);
    infoView.draw(screen);
	navView.draw(screen);
}

SDL_Rect MainView::boundary() const {
    return boundaryMainView;

}

void MainView::handleResize(SDL_Surface* screen) {
    boundaryMainView = SDL_Rect{0, 0, screen->w, screen->h};
}

void MainView::setCamera(const Basis& newCamera) {
    camera = newCamera;
    moonView.setCamera(newCamera);
}

Basis MainView::getCamera() const {
    return this->camera;
}

const Grid& MainView::getGrid() const {
    return moonView.getGrid();
}
