#include "INC_SDL.h"
#include "Player.h"
#include "Enemy.h"
#include "attack.h"
#include "OpenGLRenderer.hpp"
#include <iostream>

class AlcoholCloud
{
	public:
		AlcoholCloud(Player* p, Enemy* e, attack* atk);
		void Render();
		bool Seen();
		void setDelay(int time);
		int getDelay();
		void setYPosition(int y);

	private:
		int frame;
		int delay;
		int flareTime;
		//~ SDL_Texture* sprite;
		//~ SDL_Texture* spriteFlare;

		const char *cloudTexture;
		const char *flareUpTexture;

		SDL_Rect spriteBox;
		SDL_Rect alcoholCam;
		SDL_Rect surroundingAlcoholCam;
		SDL_Rect* playerCam;
		SDL_Rect* enemyCam;
		Player* ply;
		Enemy* emy;
		OpenGLRenderer* openGL;
		attack* plyBlast;
		bool onScreen;
		bool flareUp;

		RenderObject* cloudRender;
		RenderObject* flareUpRender;

		//Check collision with the player
		void checkPlayerCollision();

		//Check collision with the enemy
		void checkEnemyCollision();
};
