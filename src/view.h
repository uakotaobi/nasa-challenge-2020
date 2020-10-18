#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <SDL.h>

class View {
public:
    View();
    virtual void draw(SDL_Surface* screen) = 0;
    virtual SDL_Rect boundary() const = 0;
    
};

#endif // VIEW_H_INCLUDED
