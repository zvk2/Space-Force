#include "INC_SDL.h"
#include "Player.h"
class Magnetar
{
	public:
		Magnetar(Player* main, SDL_Texture* gIm, SDL_Texture* textIm);
		void Render();
		void Multiplayer(Player* player2);
		bool Seen();
		
		
	private:
	int frame;
	SDL_Texture* im;
	SDL_Texture* text;
	SDL_Rect collBox;
	SDL_Rect camBox;
	SDL_Rect textBox;
	SDL_Rect textScreen;
	SDL_Rect* playerCam;
	SDL_Rect* player2Cam;
	Player* ply;
	Player* ply2;
	SDL_Renderer* gRenderer;
	bool onScreen;
	bool multiplayer;
	int textH;
	int textW;
};
