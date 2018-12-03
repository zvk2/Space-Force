#include "INC_SDL.h"
#include "Player.h"
#include "OpenGLRenderer.hpp"
class Magnetar
{
	//~ static constexpr char magnetarTexture[] = "resources/imgs/magnetar";

	public:
		//~ Magnetar(Player* main, SDL_Texture* gIm);
		Magnetar(Player* main);
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
	OpenGLRenderer* openGL;
	RenderObject* render;
	bool onScreen;
	bool multiplayer;
};
