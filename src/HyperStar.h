#include "INC_SDL.h"
#include "Player.h"
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
		HyperStar(SDL_Texture* im, Player* main);
		HyperStar();
		void addStar();
		void Render(double timestep);
		bool checkCol(SDL_Rect circle);
	
	private:
		SDL_Texture* starIm;
		Player* ply;
		SDL_Rect imBox;
		SDL_Rect* plyCam;
		SDL_Renderer* gRenderer;
		struct StarNode* head;
		struct StarNode* end;
};
