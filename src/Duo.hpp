#pragma once

#ifndef DUO_HPP
#define DUO_HPP
#include "INC_SDL.h"
#include "OpenGLRenderer.hpp"
#include "Player.h"
#include <ctime>

class Duo
{
	public:
		Duo(OpenGLRenderer *gRenderer, Player* p);
		void Move();

	private:
		Player* ply;
		OpenGLRenderer* openGL;

		SDL_Rect miasmaRect;

		RenderObject* renderMiasma;
		RenderObject* renderChatterBox;
		RenderObject* renderHarbinger;

		GLfloat currentRot;

		//~ mat4 harbingerYRot;

		//~ mat4 chatterBoxXRot;
		//~ mat4 chatterBoxYRot;
		//~ mat4 chatterBoxZRot;

		void GetNewLocation();
};

#endif
