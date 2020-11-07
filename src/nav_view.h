#ifndef NAV_VIEW_H_INCLUDED
#define NAV_VIEW_H_INCLUDED

#include "view.h"

class NavView : public View {
	public:
        NavView(SDL_Rect navBoundary, int deltaX, int deltaY);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
    private:
        SDL_Rect boundaryNavView;
};




#endif // MOON_NAV_H_INCLUDED
