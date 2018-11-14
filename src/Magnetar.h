#include "INC_SDL.h"
#include "Player.h"
class Magnetar
{
	public:
		Magnetar(Player* main, SDL_Texture* gIm);
		void Render();
		void Multiplayer(Player* player2);
		bool Seen();
		
		
	private:
	int frame;
	SDL_Texture* im;
	SDL_Rect collBox;
	SDL_Rect camBox;
	SDL_Rect* playerCam;
	SDL_Rect* player2Cam;
	Player* ply;
	Player* ply2;
	SDL_Renderer* gRenderer;
	bool onScreen;
	bool multiplayer;
};
