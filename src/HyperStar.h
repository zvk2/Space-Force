#include "INC_SDL.h"
#include "Player.h"
#include "music.h"
class HyperStar
{
	struct StarNode
	{
		HyperStar on();
		int math;//0 = down 1 = up
		int frame;
		double angle;
		double vel;
		double x;
		double y;
		SDL_Rect colTest;
		struct StarNode* next;
		struct StarNode* pre;
		bool hitPly;
	};
	public:
		HyperStar(SDL_Texture* im, Player* main, music* mus);
		HyperStar();
		void addStar();
		void Render(double timestep);
		bool checkCol(SDL_Rect other, SDL_Rect star);//other is the object that isn't the star
		bool checkCirCol(SDL_Rect circle);
		bool checkShieldCol(SDL_Rect circle);
		void killStar();
	
	private:
		int getShield;
		bool* hasShield;
		SDL_Rect* shield;
		SDL_Texture* starIm;
		Player* ply;
		music* play;
		SDL_Rect imBox;
		SDL_Rect* plyCam;
		SDL_Renderer* gRenderer;
		struct StarNode* head;
		struct StarNode* curr;
		struct StarNode* end;
		int count;
		int countKill;
};
