
#include "GameOver.h"

// OpenGLified 2D menu w/ SDL
/*
	Displays GAME OVER message, buttons CREDITS and QUIT
	CREDITS plays credits, QUIT exits out of the entire program
*/

GameOver::GameOver(OpenGLRenderer *openGLInit)
{
	openGL = openGLInit;

	background = new RenderObject(
		0, 0, -1, openGL->allBufferAttributes["resources/imgs/game_over.png"]
	);
	openGL->AppendRenderObject(background);
	creditsButton = new RenderObject(
		SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/2 + 25, -1, openGL->allBufferAttributes["resources/imgs/credits.png"]
	);
	openGL->AppendRenderObject(creditsButton);
	quitButton = new RenderObject(
		SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/2 + 100, -1, openGL->allBufferAttributes["resources/imgs/quit.png"]
	);
	openGL->AppendRenderObject(quitButton);
}

int GameOver::runScreen()
{
	bool running = true;
	/*
		option: returned by runMenu
		0 = quit program (if SDL_EXIT)
		1 = play credits (if Credits selected)
		2 = quit program (if QUit selected)
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
				/*/ 'Menu' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/2 && y <= SCREEN_HEIGHT/2 + BUTTON_HEIGHT)
				{
					menuButton->currentBufferID = menuButton->bufferAttributes.bufferIDEnd;
				}
				else if (menuButton->currentBufferID == menuButton->bufferAttributes.bufferIDEnd)
				{
					menuButton->currentBufferID = menuButton->bufferAttributes.bufferIDStart;
				}*/
				// 'Credits' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/2 + 25 && y <= SCREEN_HEIGHT/2 + BUTTON_HEIGHT + 50)
				{
					creditsButton->currentBufferID = creditsButton->bufferAttributes.bufferIDEnd;
				}
				else if (creditsButton->currentBufferID == creditsButton->bufferAttributes.bufferIDEnd)
				{
					creditsButton->currentBufferID = creditsButton->bufferAttributes.bufferIDStart;
				}
				// 'Quit' button animation using coordinates
				if (x >= SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x <= SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y >= SCREEN_HEIGHT/2 + 100 && y <= SCREEN_HEIGHT/2 + BUTTON_HEIGHT + 100)
				{
					quitButton->currentBufferID = quitButton->bufferAttributes.bufferIDEnd;
				}
				else if (quitButton->currentBufferID == quitButton->bufferAttributes.bufferIDEnd)
				{
					quitButton->currentBufferID = quitButton->bufferAttributes.bufferIDStart;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				/*if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/5 && y < SCREEN_HEIGHT/5 + BUTTON_HEIGHT)
				{
					// 'Menu' button clicked
					running = false;
					option = 1;
					break;
				}*/

				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/2 + 25 && y < SCREEN_HEIGHT/2 + BUTTON_HEIGHT + 50)
				{
					// 'Credits' button clicked
					running = false;
					option = 1;
					break;
				}

				if (x > SCREEN_WIDTH/2 - BUTTON_WIDTH/2 && x < SCREEN_WIDTH/2 + BUTTON_WIDTH/2 && y > SCREEN_HEIGHT/2 + 100 && y < SCREEN_HEIGHT/2 + BUTTON_HEIGHT + 100)
				{
					// 'Quit' button clicked
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

