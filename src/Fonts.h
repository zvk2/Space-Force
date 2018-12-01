#include "INC_SDL.h"
#include <iostream>
#include <SDL_ttf.h>
class Fonts
{
	public:
		Fonts(SDL_Renderer* rend);
		bool RenderFont(std::string words);
		void CloseFont();
		
		
	private:
		bool init();
		bool load_files();
		SDL_Renderer* gRenderer;
		TTF_Font *font;
		SDL_Color colorText;
		SDL_Surface *message;
		SDL_Texture * text;
		
};
