#ifndef BUTTON_VIEW_H_INCLUDED
#define BUTTON_VIEW_H_INCLUDED

#include <SDL.h>
#include "view.h"
#include <string>

class ButtonView : View {
    public:
        ButtonView(SDL_Rect buttonBoundary, std::string buttonText, SDL_Color buttonColor);
        ~ButtonView();
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
    private:
        SDL_Rect buttonBoundary_;
        std::string buttonText_;
        SDL_Color buttonColor_;
};

#endif // BUTTON_VIEW_H_INCLUDED