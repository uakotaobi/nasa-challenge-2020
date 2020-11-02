#include "main_view.h"
#include "asset_manager.h"

MainView::MainView(SDL_Surface* screen) {
		boundaryMainView = SDL_Rect{0, 0, screen->w, screen->h};
};

void MainView::draw(SDL_Surface* screen) {
		SDL_FillRect(screen, &boundaryMainView, SDL_MapRGB(screen->format, 0, 0, 0));
};

SDL_Rect MainView::boundary() const {
		return boundaryMainView;

};

void MainView::handleResize(SDL_Surface* screen) {
		boundaryMainView = SDL_Rect{0, 0, screen->w, screen->h};
}
