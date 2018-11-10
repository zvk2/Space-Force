#include "INC_SDL.h"
#include "Player.h"
class Magnetar
{
	public:
		Magnetar(Player* main, SDL_Texture* gIm);
		void Render();
		bool Seen();
		
		
	private:
	int frame;
	SDL_Texture* im;
	SDL_Rect collBox;
	SDL_Rect camBox;
	SDL_Rect* playerCam;
	Player* ply;
	SDL_Renderer* gRenderer;
	bool onScreen;
};
