#ifndef ATTACK_H
#define ATTACK_H
#include<stdio.h> 
#include<stdlib.h> 
#include "INC_SDL.h"
class attack
{
		struct Node
		{
			SDL_Rect attackCam;
			struct Node *next;
		};
	public:
		attack(SDL_Renderer* Renderer, SDL_Texture* gAtt, SDL_Rect* attac,SDL_Rect cam);
		void addAttack(SDL_Rect cam);
		void renderAttack(double timestep);
	private:
		struct Node* head;
		struct Node* end;
		struct Node* curr;
		SDL_Rect* attackBox;
		SDL_Renderer* gRenderer;
		SDL_Texture* gAttack;
};
#endif