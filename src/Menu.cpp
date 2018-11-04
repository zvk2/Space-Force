
#include "Menu.h"

// OpenGLified 2D menu w/ SDL
/* 
	Displays titlescreen and buttons START, MULTIPLAYER, and CREDITS
	START goes straight to game, MULTIPLAYER does nothing at the moment, CREDITS instructs main to play credits

*/

Menu::Menu(): start_clip_begin(0), start_clip_end(0.5), multi_clip_begin(0), multi_clip_end(0.5), credits_clip_begin(0), credits_clip_end(0.5) {}

int Menu::load(const char* filename)
{
	SDL_Surface* image = IMG_Load(filename);
	if (image == nullptr)
	{
		std::cout << "Unable to load image " << filename << "! SDL_IMG Error: " << IMG_GetError() << std::endl;
		return -1;
	}
	GLuint object;
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glGenTextures(1, &object);
	glBindTexture(GL_TEXTURE_2D, object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	SDL_FreeSurface(image);
	return object;
}

void Menu::displayMenu(SDL_Window* window)
{
	gWindow = window;
	gContext = SDL_GL_CreateContext(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw background	
	texture = load("resources/imgs/titlescreen.png");
	if (texture == -1)
	{
		throw std::exception();
	}
	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(0, 1); glVertex2f(0, SCREEN_HEIGHT);
	glEnd();
	
	// Draw start button	
	start_button = load("resources/imgs/start.png");
	if (start_button == -1)
	{
		throw std::exception();
	}
	//glDepthMask(GL_TRUE);
 	glLoadIdentity();
 	glMatrixMode(GL_MODELVIEW);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
 	
	glBegin(GL_QUADS);
 	glTexCoord2f(start_clip_begin, 0); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), SCREEN_HEIGHT/5);
 	glTexCoord2f(start_clip_end, 0); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), SCREEN_HEIGHT/5);
 	glTexCoord2f(start_clip_end, 1); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT));
	glTexCoord2f(start_clip_begin, 1); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT));
	glEnd();
	
	// Draw multiplayer button
	multi_button = load("resources/imgs/multi.png");
	if (multi_button == -1)
	{
		throw std::exception();
	}
	//glDepthMask(GL_TRUE);
 	glLoadIdentity();
 	glMatrixMode(GL_MODELVIEW);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
 	
	glBegin(GL_QUADS);
 	glTexCoord2f(multi_clip_begin, 0); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), SCREEN_HEIGHT/5 + 50);
 	glTexCoord2f(multi_clip_end, 0); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), SCREEN_HEIGHT/5 + 50);
 	glTexCoord2f(multi_clip_end, 1); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50));
	glTexCoord2f(multi_clip_begin, 1); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50));
	glEnd();
	
	// Draw credits button
	credits_button = load("resources/imgs/credits.png");
	if (credits_button == -1)
	{
		throw std::exception();
	}
	//glDepthMask(GL_TRUE);
 	glLoadIdentity();
 	glMatrixMode(GL_MODELVIEW);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
 	
	glBegin(GL_QUADS);
 	glTexCoord2f(credits_clip_begin, 0); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), SCREEN_HEIGHT/5 + 100);
 	glTexCoord2f(credits_clip_end, 0); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), SCREEN_HEIGHT/5 + 100);
 	glTexCoord2f(credits_clip_end, 1); glVertex2f((SCREEN_WIDTH/2 + BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 100));
	glTexCoord2f(credits_clip_begin, 1); glVertex2f((SCREEN_WIDTH/2 - BUTTON_WIDTH/2), (SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 100));
	glEnd();
	
	
	glFlush();
	SDL_GL_SwapWindow(window);
}

void Menu::closeMenu()
{
	SDL_GL_DeleteContext(gContext);
	//SDL_DestroyWindow(gWindow);
	//SDL_Quit();
}

int Menu::runMenu(SDL_Window* window)
{
	bool running = true;
	/*
		option: returned by runMenu
		0 = quit program (if SDL_EXIT)
		1 = continue to game (if Start selected)
		2 = play credits (if Credits selected)
	*/
	int option = 0;

	bool s_highlight = false;
	bool m_highlight = false;
	bool c_highlight = false;
	SDL_Event event;
	while (running)
	{

		while (SDL_PollEvent(&event))
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
			int x = event.motion.x;
			int y = event.motion.y;
			if (event.type == SDL_QUIT)
			{
				option = 0;
				running = false;
				break;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				/*std::cout << "x: " << x << " y: " << y << std::endl;
				printf("x %d > %d, x %d < %d, y %d > %d, y %d < %d\n", 
					x, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 
					x, SCREEN_WIDTH/2 + BUTTON_WIDTH/2, 
					y, SCREEN_HEIGHT/5, 
					y, SCREEN_HEIGHT/5 + BUTTON_HEIGHT);
				*/
				// 'Start' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
				{
					start_clip_begin = 0.5;
					start_clip_end = 1;
					s_highlight = true;
				}	
				else if (s_highlight)
				{
					start_clip_begin = 0;
					start_clip_end = 0.5;
					s_highlight = false;	
				}
				// 'Multiplayer' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 + 50 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50)
				{
					multi_clip_begin = 0.5;
					multi_clip_end = 1;
					m_highlight = true;
				}	
				else if (m_highlight)
				{
					multi_clip_begin = 0;
					multi_clip_end = 0.5;
					m_highlight = false;	
				}
				// 'Credits' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 + 100 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 100)
				{
					credits_clip_begin = 0.5;
					credits_clip_end = 1;
					c_highlight = true;
				}	
				else if (c_highlight)
				{
					credits_clip_begin = 0;
					credits_clip_end = 0.5;
					c_highlight = false;	
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
				{
					// 'Start' button clicked
					running = false;
					option = 1; 
					break;
				}
				/*
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 + 50 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50)
				{
					// 'Multiplayer' button clicked
					running = false;
					break;
				}
				*/
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 + 100 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 100)
				{
					// 'Credits' button clicked
					running = false;
					option = 2;
					break;
				}
				//}
			}
			
		} // end event loop
		displayMenu(gWindow);
	} // end running
	return option;
}

