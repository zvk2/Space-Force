#ifndef GAMEOVER_H
#define GAMEOVER_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "INC_SDL.h"
#include "OpenGLRenderer.hpp"

class GameOver
{
	public:

		// Constructor
		GameOver(OpenGLRenderer *openGL, bool win);

		// Run loop that responds to mouse/keyboard input
		int runScreen();
	
	private:
		OpenGLRenderer *openGL;

		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		const int BUTTON_WIDTH = 128;
		const int BUTTON_HEIGHT = 32;

		RenderObject *background; // Background
		RenderObject *menuButton; // Start button
		RenderObject *creditsButton; // Credits button
		RenderObject *quitButton; // Multiplayer button
};

#endif
