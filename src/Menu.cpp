
#include "Menu.h"

// OpenGLified 2D menu w/ SDL
/*
	Displays titlescreen and buttons START, MULTIPLAYER, and CREDITS
	START goes straight to game, MULTIPLAYER goes to early multiplayer, CREDITS instructs main to play credits

*/

Menu::Menu(OpenGLRenderer *openGLInit)
{
	openGL = openGLInit;

	background = new RenderObject(
		0, 0, -1, openGL->allBufferAttributes["resources/imgs/titlescreen.png"]
	);
	openGL->AppendRenderObject(background);
	startButton = new RenderObject(
		SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/5, -1, openGL->allBufferAttributes["resources/imgs/start.png"]
	);
	openGL->AppendRenderObject(startButton);
	multiButton = new RenderObject(
		SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/5 + 50, -1, openGL->allBufferAttributes["resources/imgs/multi.png"]
	);
	openGL->AppendRenderObject(multiButton);
	creditsButton = new RenderObject(
		SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/5 + 100, -1, openGL->allBufferAttributes["resources/imgs/credits.png"]
	);
	openGL->AppendRenderObject(creditsButton);
}

int Menu::runMenu()
{
	bool running = true;
	/*
		option: returned by runMenu
		0 = quit program (if SDL_EXIT)
		1 = continue to game (if Start selected)
		2 = play credits (if Credits selected)
	*/
	int option = 0;
	int mouse_x, mouse_y;

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
				// 'Start' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
				{
					startButton->currentBufferID = startButton->bufferAttributes.bufferIDEnd;
				}
				else if (startButton->currentBufferID == startButton->bufferAttributes.bufferIDEnd)
				{
					startButton->currentBufferID = startButton->bufferAttributes.bufferIDStart;
				}
				// 'Multiplayer' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 + 50 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50)
				{
					multiButton->currentBufferID = multiButton->bufferAttributes.bufferIDEnd;
				}
				else if (multiButton->currentBufferID == multiButton->bufferAttributes.bufferIDEnd)
				{
					multiButton->currentBufferID = multiButton->bufferAttributes.bufferIDStart;
				}
				// 'Credits' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/5 + 100 && y <= SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 100)
				{
					creditsButton->currentBufferID = creditsButton->bufferAttributes.bufferIDEnd;
				}
				else if (creditsButton->currentBufferID == creditsButton->bufferAttributes.bufferIDEnd)
				{
					creditsButton->currentBufferID = creditsButton->bufferAttributes.bufferIDStart;
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

				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 + 50 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT + 50)
				{
					// 'Multiplayer' button clicked
					running = false;
					option = 3;
					break;
				}

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

		openGL->Display();
	} // end running

	// Clear the slate
	//~ openGL->TabulaRasa();

	return option;
}
