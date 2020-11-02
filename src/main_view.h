#ifndef MAIN_VIEW_H_INCLUDED
#define MAIN_VIEW_H_INCLUDED

class MainView : public View {
	public:
		MainView(SDL_Surface* screen);
		void draw(SDL_Surface* screen);
		SDL_Rect boundary() const;
		void handleResize(SDL_Surface* screen);
	private:
		SDL_Rect boundaryMainView; 
};

#endif // MAIN_VIEW_H_INCLUDED