#pragma once

#ifndef ATTACK_H
#define ATTACK_H
#include<stdio.h>
#include<stdlib.h>
#include "INC_SDL.h"
#include "OpenGLRenderer.hpp"
#include "music.h"

// Now uses a vector to monitor attacks

struct Node
{
	SDL_Rect attackCam;
	RenderObject *render;
	struct Node *next;
	struct Node *pre;
};

struct interList
{
	int type;
	SDL_Rect* rect;
	SDL_Rect* circle;
	SDL_Rect* triangle;
};

class attack
{
	//~ static constexpr char attackTexture[] = "resources/imgs/attack.png";

	public:
		attack(OpenGLRenderer* Renderer);
		void addAttack(int x, int y, int image);
		//~ void addAttack(SDL_Rect* attackBox);
		void renderAttack(double timestep, int goLeft);
		void setAttack(SDL_Rect* attac);
		int hitIntersect(SDL_Rect* rect);
		void setColSound(music* sounds);
        struct Node* getHead();
	private:
		struct Node* head;
		struct Node* end;
		struct Node* curr;
		SDL_Rect cam;
		SDL_Rect* attackBox;
		OpenGLRenderer* openGL;
		SDL_Texture* gAttack;
		music* mus;

		const char* attackTexture;

		std::vector<RenderObject*> attackVector;
};
#endif
