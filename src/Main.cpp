#include <iostream>
#include "SDL.h"

int main() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
	
	}
	SDL_Quit();
		
    
    std::cout << "Program finished.\n";
}
