#ifndef MAIN_VIEW_H_INCLUDED
#define MAIN_VIEW_H_INCLUDED

#include "view.h"
#include "moon_view.h"
#include "info_view.h"
#include "nav_view.h"

class MainView : public View {
    public:
        MainView(SDL_Surface* screen);
        void draw(SDL_Surface* screen);
        void drawWithRenderer(const Renderer& r) const;
        SDL_Rect getRenderBoundary() const; // TODO: Return moonview.boundary
        SDL_Rect boundary() const;
        void handleResize(SDL_Surface* screen);
        void setCamera(const Basis& newCamera);
        Basis getCamera() const;
        Grid& getGrid();
    private:
        SDL_Rect boundaryMainView;
        MoonView moonView;
        InfoView infoView;
		NavView navView;
        Basis camera;
};



#endif // MAIN_VIEW_H_INCLUDED
