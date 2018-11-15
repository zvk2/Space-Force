#include "INC_SDL.h"
#include "Player.h"
#include "Enemy.h"

class AlcoholCloud
{
	public:
		AlcoholCloud(Player* p, Enemy* e, SDL_Texture* i, SDL_Texture* f);
		void Render();
		bool Seen();
		void setDelay(int time);
		int getDelay();
		void setYPosition(int y);
		
	private:
		int frame;
		int delay;
		SDL_Texture* sprite;
		SDL_Texture* spriteFlare;
		SDL_Rect spriteBox;
		SDL_Rect alcoholCam;
		SDL_Rect surroundingAlcoholCam;
		SDL_Rect* playerCam;
		SDL_Rect* enemyCam;
		Player* ply;
		Enemy* emy;
		SDL_Renderer* gRenderer;
		bool onScreen;
		
		//Check collision with the player
		void checkPlayerCollision();
		
		//Check collision with the enemy
		void checkEnemyCollision();
};