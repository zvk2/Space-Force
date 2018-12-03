#ifndef SHIELD_H
#define SHIELD_H
#pragma once
#include "INC_SDL.h"
#include "Player.h"
#include "OpenGLRenderer.hpp"
class Shield
{
	public:
		Shield(Player* main);
		void Render();
		void RenderPower();
		void NewItem();
		void Damage(int hitsTaken);
	private:
		OpenGLRenderer* openGL;
		//~ SDL_Texture* item;
		//~ SDL_Texture* protect;

		const char* itemTexture;
		const char* protectTexture;

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

		RenderObject* renderItem;
		RenderObject* renderProtect;
};
#endif
