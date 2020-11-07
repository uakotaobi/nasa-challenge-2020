#ifndef INFO_VIEW_H_INCLUDED
#define INFO_VIEW_H_INCLUDED

#include <SDL.h>
#include "view.h"
#include <string>
#include <functional>

class InfoView : public View {
    public:
        InfoView(SDL_Rect infoBoundary, int deltaX, int deltaY);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        // void handleResize(SDL_Surface* screen);
    private:
        SDL_Rect boundaryInfoView;
        std::string infoText_;
        SDL_Surface* renderText;
};




#endif // INFO_VIEW_H_INCLUDED
