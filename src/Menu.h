#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "INC_SDL.h"

class Menu
{
	public:

		// Constructor
		Menu();

		// Create GLuint object from SDL_Surface
		int load(const char* filename);

		// Display menu background and buttons 
		void displayMenu(SDL_Window* window);

		// Run menu loop that responds to mouse/keyboard input
		int runMenu(SDL_Window* window);

		// Close the menu
		void closeMenu();
	
		/* To be added later(?) 
		
		- multiplayer method

		- options method

		*/
	private:
		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		const int BUTTON_WIDTH = 128;
		const int BUTTON_HEIGHT = 32;

		SDL_Window* gWindow;
		SDL_GLContext gContext;
		GLuint texture; // Background
		GLuint start_button; // Start button
		GLuint multi_button; // Multiplayer button
		GLuint credits_button; // Credits button
		int mouse_x, mouse_y;
		GLfloat start_clip_begin, start_clip_end, multi_clip_begin, multi_clip_end, credits_clip_begin, credits_clip_end;

};

#endif
