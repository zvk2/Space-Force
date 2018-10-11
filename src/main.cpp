// This file iterates over every listed file in the folder "Credit_Image" and displays it for 3 seconds
// This uses "sdl0_hello.cpp" as boilerplate

// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>

// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

//physic 
#include "physics.hpp"

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int LEVEL_LEN = 3840;

//temp stand in for spaceship
constexpr int BOX_WIDTH = 200;
constexpr int BOX_HEIGHT = 200;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gBackground;
SDL_Texture* gItems;
std::vector<SDL_Texture*> gTex;

bool init()
{
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	
	gWindow = SDL_CreateWindow(
		"Space Force",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			
	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags)
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	
	return true;
}

SDL_Texture* loadImage(std::string fname)
{
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr)
	{
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr)
	{
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void close()
{
	for (auto i : gTex)
	{
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

// Display the credits.
int playCredits()
{
	SDL_RenderClear(gRenderer);
	// Load media by iterating over the files in "Credit_Image"	
	struct dirent *entry;
	DIR *dp;
	
	dp = opendir(CREDITS_FOLDER);
	if (dp == NULL)
	{
		perror("opendir: Path does not exist or could not be read.");
		return -1;
	}
	SDL_Event e;
	while ((entry = readdir(dp))&&e.type != SDL_QUIT)
	{
		SDL_PollEvent(&e);
		int dNameLength = strlen(entry->d_name);
		
		// Crude
		if (dNameLength > 4 && entry->d_name[dNameLength - 4] == '.')
		{
			// Crude
			char currentImageBuffer[2000];
			strcpy(currentImageBuffer, CREDITS_FOLDER);
			strcat(currentImageBuffer, entry->d_name);
			
			// Puts the image on the buffer queue
			gTex.push_back(loadImage(currentImageBuffer));
		}
	}
	
	// Close the directory
	closedir(dp);
	
	for (auto i : gTex)
	{
		// Clear
		SDL_RenderClear(gRenderer);
		// Render the image
		SDL_RenderCopy(gRenderer, i, NULL, NULL);
		// Display rendering
		SDL_RenderPresent(gRenderer);
		// Wait 3 seconds
		SDL_Delay(3000);
	}

	// Clear the renderer one last time
	SDL_RenderClear(gRenderer);
	close();
}

int main(int argc, char* argv[])
{	
	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	
	SDL_Texture* background = loadImage("resources/imgs/space_background.png");
	int scroll_offset = 0;
	int rem = 0;
	int c = 0;
	
	//Player rectangle
	SDL_Texture* p = loadImage("resources/Credit_Image/Credit_AnthonyMartrano.png");
	SDL_Rect pRect = {SCREEN_WIDTH/5 - BOX_WIDTH/2, SCREEN_HEIGHT/2 - BOX_HEIGHT/2, 200, 200};
	SDL_Rect pSpriteRect = {200, 200, 200, 200};
	int x_vel = 0;
	int y_vel = 0;
	
	SDL_Rect pCam = {SCREEN_WIDTH/5 - BOX_WIDTH/2, SCREEN_HEIGHT/2 - BOX_HEIGHT/2, 200, 200};
	SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	//change in velocity
	int x_deltav = 0;
	int y_deltav = 0;
	Physics playerPysics(&x_vel,&y_vel,&x_deltav,&y_deltav,3,1,1);
	
	//Enemy rectangle
	SDL_Texture* b = loadImage("resources/Credit_Image/Ling.png");
	SDL_Rect bRect = {SCREEN_WIDTH/2 - BOX_WIDTH/2, SCREEN_HEIGHT/2 - BOX_HEIGHT/2, 200, 200};
	SDL_Rect bSpriteRect = {150, 300, 200, 200};
	
	SDL_Rect moveBox = {SCREEN_WIDTH/2-BOX_WIDTH/2,SCREEN_HEIGHT/2-BOX_HEIGHT/2,BOX_WIDTH,  BOX_HEIGHT};
	
	int lthird = (SCREEN_WIDTH/3) - BOX_WIDTH/2;
	int rthird = (2 * SCREEN_WIDTH/3) - BOX_WIDTH/2;
	

	
	int b_xvel = 1;
	
	SDL_Event e;
	
	bool gameon = true;
	
	while(gameon)
	{
		//get array of current key states
		const Uint8* keystate = SDL_GetKeyboardState(nullptr);
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || keystate[SDL_SCANCODE_ESCAPE])
			{
				gameon = false;
			}
		}
		x_deltav = 0;
		y_deltav = 0;
		
		if (keystate[SDL_SCANCODE_W])
		{
			y_deltav -= 1;
		}
		if (keystate[SDL_SCANCODE_A])
		{
			x_deltav -= 1;
		}
		if (keystate[SDL_SCANCODE_S])
		{
			y_deltav += 1;
		}
		if (keystate[SDL_SCANCODE_D])
		{
			x_deltav += 1;
		}
		SDL_Delay(3);
		//when none of the movement keys are pressed
		playerPysics.Neutral();
		// 
		// Speed up/slow down
		x_vel += x_deltav;

		y_vel += y_deltav;
		
		//the speed limit for the character
		playerPysics.Speed_Limit();
			
		// Move box
		// Try to move vertically
		pRect.y += y_vel;
		if (pRect.y < 0 || (pRect.y + BOX_HEIGHT > SCREEN_HEIGHT) || SDL_HasIntersection(&bRect, &pRect))
		{
			pRect.y -= y_vel;
		}

		// Try to move horizontally
		pRect.x += x_vel;
		if (pRect.x < 0 || (pRect.x + BOX_WIDTH > LEVEL_LEN) || SDL_HasIntersection(&bRect, &pRect))
		{
			pRect.x -= x_vel;

		}
		
		bRect.x += b_xvel;
		//Move the enemy rectangle to the left if it reaches the right of the screen
		if (bRect.x + BOX_WIDTH > SCREEN_WIDTH)
		{
			bRect.x -= b_xvel;
			b_xvel = -1;
		}
		//Move the enemy rectangle to the right if it reaches the left of the screen
		if (bRect.x < 0)
		{
			bRect.x -= b_xvel;
			b_xvel = 1;
		}
		if(pRect.x> (scroll_offset+rthird))
		{
			scroll_offset = pRect.x - rthird;
		}
		else if(pRect.x < (scroll_offset + lthird))
		{
			scroll_offset = pRect.x-lthird;
		}
		if (scroll_offset < 0)
		{
			scroll_offset = 0;
		}
		if (scroll_offset + SCREEN_WIDTH> LEVEL_LEN)
		{
			scroll_offset = LEVEL_LEN - SCREEN_WIDTH;
		}
		
		//Check to see if the enemy rectangle colides with the player rectangle
		if (SDL_HasIntersection(&bRect, &pRect))
		{
			bRect.x -= b_xvel;
		}

		SDL_RenderClear(gRenderer);
		rem = scroll_offset % SCREEN_WIDTH;
		bgRect.x = -rem;

		SDL_RenderCopy(gRenderer, background, NULL, &bgRect);
		bgRect.x+= SCREEN_WIDTH;
		SDL_RenderCopy(gRenderer, background, NULL, &bgRect);
		
		pCam.x = pRect.x - scroll_offset;
		pCam.y = pRect.y;
		SDL_RenderCopy(gRenderer, p, &pSpriteRect, &pCam);
		SDL_RenderCopy(gRenderer, b, &bSpriteRect, &bRect);
		
		SDL_RenderPresent(gRenderer);
	}
	
	return playCredits();
}

