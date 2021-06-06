#ifndef MAIN_VIEW_H_INCLUDED
#define MAIN_VIEW_H_INCLUDED

#include "view.h"
#include "moon_view.h"
#include "info_view.h"
#include "nav_view.h"
#include "render.h"
#include "fps_view.h"

class MainView : public View {
    public:
        MainView(SDL_Surface* screen);
        void draw(SDL_Surface* screen);
        void drawWithRenderer(const Renderer& r);
        SDL_Rect getRenderBoundary() const;
        SDL_Rect boundary() const;
        void handleResize(SDL_Surface* screen);
        void setCamera(const Basis& newCamera);
        Basis getCamera() const;
        Grid& getGrid();
        void updateFps(double averageFps);
    private:
        SDL_Rect boundaryMainView;
        MoonView moonView;
        InfoView infoView;
		NavView navView;
        Basis camera;
        FrameRateView frameRateView;
};



#endif // MAIN_VIEW_H_INCLUDED
