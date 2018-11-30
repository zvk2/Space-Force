#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "INC_SDL.h"
#include "OpenGLRenderer.hpp"

class Menu
{
	public:
		// Constructor
		Menu(OpenGLRenderer *openGL);

		// Run menu loop that responds to mouse/keyboard input
		int runMenu();
	private:
		OpenGLRenderer *openGL;

		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		const int BUTTON_WIDTH = 128;
		const int BUTTON_HEIGHT = 32;

		RenderObject *background; // Background
		RenderObject *startButton; // Start button
		RenderObject *multiButton; // Multiplayer button
		RenderObject *creditsButton; // Credits button
};

#endif
