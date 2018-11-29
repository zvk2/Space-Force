#ifndef SHIELD_H
#define SHIELD_H
#pragma once
#include "INC_SDL.h"
#include "Player.h"
class Shield
{
	public:
		Shield(SDL_Texture* imItem, SDL_Texture* power, Player* main);
		void Render();
		void RenderPower();
		void NewItem();
	private:
		SDL_Renderer* gRenderer;
		SDL_Texture* item; 
		SDL_Texture* protect; 
		SDL_Rect itemLoc;
		SDL_Rect itemIm;
		SDL_Rect protectLoc;
		SDL_Rect protectIm;
		Player* ply;
		SDL_Rect* plyCam;
		bool itemScreen;
		bool screen;
		int hits;
		int addStrength;
};
#endif