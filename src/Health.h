#ifndef HEALTH_H
#define HEALTH_H
#pragma once
#include "INC_SDL.h"
#include "Player.h"

class Health
{
	public:
		Health(SDL_Texture* imItem, Player* main);
		void Render();
		void NewItem();

	private:
		SDL_Renderer* gRenderer;
		SDL_Texture* item;
		SDL_Rect itemLoc;
		SDL_Rect itemIm;
		SDL_Rect healthLoc;
		SDL_Rect healthIm;
		Player* ply;
		SDL_Rect* plyCam;
		bool itemScreen;
		bool screen;
};

#endif