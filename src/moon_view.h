#ifndef MOON_VIEW_H_INCLUDED
#define MOON_VIEW_H_INCLUDED

#include "view.h"

class MoonView : public View {
    public:
        MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
    private:
        SDL_Rect boundaryMoonView;
};




#endif // MOON_VIEW_H_INCLUDED
