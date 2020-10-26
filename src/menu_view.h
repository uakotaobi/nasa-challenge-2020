#ifndef MENU_VIEW_H_INCLUDED
#define MENU_VIEW_H_INCLUDED

#include <SDL.h>
#include "view.h"
#include "button_view.h"

class MenuView : public View {
    public:
        MenuView(SDL_Surface* surf, bool& done);
        void draw(SDL_Surface* screen);
        SDL_Rect boundary() const;
        void handleClicks(SDL_MouseButtonEvent& mouseButtonEvent);

    private:
        ButtonView startButton;
        ButtonView quitButton;
};


#endif // MENU_VIEW_H_INCLUDED
