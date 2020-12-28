#ifndef MOON_VIEW_H_INCLUDED
#define MOON_VIEW_H_INCLUDED

#include "view.h"
#include "grid.h"
#include "basis.h"

class MoonView : public View {
    public:
        MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
        void setCamera(const Basis& newCamera);
        
    private:
        SDL_Rect boundaryMoonView;
        Grid moonGrid;
        Basis camera;
};

#endif // MOON_VIEW_H_INCLUDED
