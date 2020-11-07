#ifndef MAIN_VIEW_H_INCLUDED
#define MAIN_VIEW_H_INCLUDED

#include "view.h"
#include "moon_view.h"
#include "info_view.h"

class MainView : public View {
    public:
        MainView(SDL_Surface* screen);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        void handleResize(SDL_Surface* screen);
    private:
        SDL_Rect boundaryMainView;
        MoonView moonView;
        InfoView infoView;
};

#endif // MAIN_VIEW_H_INCLUDED
