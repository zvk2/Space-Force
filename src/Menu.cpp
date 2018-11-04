
#include "Menu.h"

// OpenGLified 2D menu w/ SDL
/* 
	Currently just displays titlescreen and 2 buttons, START and MULTIPLAYER
	START goes straight to game, MULTIPLAYER does nothing at the moment
	Anyone else (esp. 3D crew) is free to edit my OpenGL noob code if there are better/less cumbersome implementations compared to what I did   
		- Kevin
*/

Menu::Menu(): start_clip_begin(0), start_clip_end(0.5), multi_clip_begin(0), multi_clip_end(0.5) {}

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
	
	glFlush();
	SDL_GL_SwapWindow(window);
}

void Menu::closeMenu()
{
	SDL_GL_DeleteContext(gContext);
	//SDL_DestroyWindow(gWindow);
	//SDL_Quit();
}

void Menu::runMenu(SDL_Window* window)
{
	bool running = true;
	bool s_highlight = false;
	bool m_highlight = false;
	SDL_Event event;
	while (running)
	{

		while (SDL_PollEvent(&event))
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
			int x = event.motion.x;
			int y = event.motion.y;
			if (event.type == SDL_QUIT)
				running = false;
		
			if (event.type == SDL_MOUSEMOTION)
			{
				//std::cout << "x: " << x << " y: " << y << std::endl;
				printf("x %d > %d, x %d < %d, y %d > %d, y %d < %d\n", 
					x, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 
					x, SCREEN_WIDTH/2 + BUTTON_WIDTH/2, 
					y, SCREEN_HEIGHT/5, 
					y, SCREEN_HEIGHT/5 + BUTTON_HEIGHT);
				// 'Start' button animation using coordinates
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
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
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 + 50 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50)
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
				
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
				{
					// 'Start' button clicked
					running = false;
					break;
				}
				//}
			}
			
		} // end event loop
		displayMenu(gWindow);
	} // end running
}

