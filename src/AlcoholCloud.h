#include "INC_SDL.h"
#include "Player.h"

class AlcoholCloud
{
	public:
		AlcoholCloud(Player* p, SDL_Texture* i);
		void Render();
		bool Seen();
		void setDelay(int time);
		int getDelay();
		void setYPosition(int y);
		
	private:
		int frame;
		int delay;
		SDL_Texture* sprite;
		SDL_Rect spriteBox;
		SDL_Rect alcoholCam;
		SDL_Rect surroundingAlcoholCam;
		SDL_Rect* playerCam;
		Player* ply;
		SDL_Renderer* gRenderer;
		bool onScreen;
};