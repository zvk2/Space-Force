#pragma once

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
		struct interList
		{
			int type;
			SDL_Rect* rect;
			SDL_Rect* circle;
			SDL_Rect* triangle;
		};
	public:
		attack(SDL_Renderer* Renderer);
		void addAttack(int x, int y);
		void renderAttack(double timestep);
		void setAttack(SDL_Texture* gAtt, SDL_Rect* attac);
		int hitIntersect(SDL_Rect* rect);
		Node* attackList();
	private:
		struct Node* head;
		struct Node* end;
		struct Node* curr;
		SDL_Rect cam;
		SDL_Rect* attackBox;
		SDL_Renderer* gRenderer;
		SDL_Texture* gAttack;
};
#endif