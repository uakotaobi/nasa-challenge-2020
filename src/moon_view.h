#ifndef MOON_VIEW_H_INCLUDED
#define MOON_VIEW_H_INCLUDED

#include "view.h"
#include "grid.h"
#include "basis.h"
#include "render.h"

class MoonView : public View {
    public:
        MoonView(SDL_Rect moonBoundary, int deltaX, int deltaY);

        // This function no longer works, it just draws a black rectangle. If
        // you want to draw the moon, call drawWithRenderer().
        void draw(SDL_Surface* screen);

        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
        void setCamera(const Basis& newCamera);
        Grid& getGrid();

        // Allows the moon_view to draw using a renderer instead of an
        // SDL_Surface because our moon grid can no longer render with an
        // SDL_Surface.
        void drawWithRenderer(const Renderer& r) const;

    private:
        SDL_Rect boundaryMoonView;
        Grid moonGrid;
        Basis camera;
};

#endif // MOON_VIEW_H_INCLUDED
