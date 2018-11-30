// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "INC_SDL.h"
#include "Magnetar.h"
#include "AlcoholCloud.h"
#include "Player.h"
#include "Enemy.h"
#include "attack.h"
#include "blackhole.h"
#include "Menu.h"
#include "ClientInterface.h"
#include <cstdlib>
#include "HyperStar.h"
#include "music.h"
#include "Shield.h"

#include "OpenGLRenderer.hpp"

// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

// Constants for resolution
// Now declared globally in OpenGLRenderer.hpp
//~ const int SCREEN_WIDTH = 1280;
//~ const int SCREEN_HEIGHT = 720;

// Constants for level
const int LEVEL_LEN = 5120;
const int TILE_SIZE = 100;

// Constant for acceleration
//const double ACCEL = 3600.0;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

//The context that is used to render OpenGL to the SDL Window
SDL_GLContext gContext;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
int close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gBackground;
SDL_Texture* gAttack;
SDL_Texture* gBlackhole;
SDL_Texture* gPlayerSheet;
SDL_Texture* gHealthbar;
std::vector<SDL_Texture*> gTex;
ClientInterface* client;

music mus;

bool init()
{
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
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
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	client = new ClientInterface();


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

int close()
{
	//~ for (auto i : gTex)
	//~ {
		//~ SDL_DestroyTexture(i);
		//~ i = nullptr;
	//~ }

	mus.close();

	SDL_DestroyRenderer(gRenderer);
    SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	return 0;
}

// CREDITS
int playCredits(OpenGLRenderer *openGL)
{
	// Your compiler may complain because you are not supposed to do this
	// Credits to play
	std::string creditNames[] = {
		"resources/Credit_Image/carolyn_cole.png",
		"resources/Credit_Image/Credit_AnthonyMartrano.png",
		"resources/Credit_Image/DylanUmble.png",
		"resources/Credit_Image/KevinW_credit.png",
		"resources/Credit_Image/luke_malchow_bergenthal_1_3_FINAL_last_edge_lord.png",
		"resources/Credit_Image/RuthDereje.png",
		"resources/Credit_Image/ryan-kuhn.png",
		"resources/Credit_Image/ShreeSampath.png",
		"resources/Credit_Image/Zane_Credits.png",
		"resources/Credit_Image/zhishengXu.png"
	};

	int index = 0;
	bool creditsContinue = true;
	SDL_Event e;

	while (index < 10 && creditsContinue)
	{
		const char *currentCreditName = creditNames[index].c_str();

		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				creditsContinue = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					creditsContinue = false;
				}
				if (e.key.keysym.sym == SDLK_q)
				{
					creditsContinue = false;
				}
			}
		}

		std::cout << currentCreditName << std::endl;
		RenderObject *currentCreditImage = new RenderObject(
			0, 0, 1, openGL->allBufferAttributes[currentCreditName]
		);
		openGL->AppendRenderObject(currentCreditImage);

		openGL->Display();

		// Wait 3 seconds
		SDL_Delay(3000);

		// Kill image
		openGL->RemoveRenderObject(currentCreditImage->index);

		index++;
	}

	//close();
    return 0;
}

int main(int argc, char* argv[])
{

	bool Multiplayer = false;

	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	// MENU

	OpenGLRenderer openGL = OpenGLRenderer(gWindow);

	Menu menu = Menu(&openGL);
	int selection = menu.runMenu();
	//std::cout << selection << std::endl;
	while (selection == 2)
	{
		playCredits(&openGL);
		selection = menu.runMenu();
	}
	if (selection == 0)
	{
		close();
		return 0;
	}
	if (selection == 1 || selection == 3)
	{
		if(selection == 3){
			Multiplayer = true;
		}
		cout << "SELECTION " << selection << endl;
	}

	// Now that we are ready to start the game, clean the openGLRenderer
	openGL.TabulaRasa();

	// GAME
	/*
	- Controls: WASD for movement, Spacebar to shoot
	- Can be terminated by x'ing out or pressing 'esc' to credits
	- Starman sprite's dimensions are 300 x 51
	*/
	//used to call playMusic
	mus = music();
	mus.init();
	mus.playMusic();

	gBackground = loadImage("resources/imgs/space_2_background.png");
	gAttack = loadImage("resources/imgs/attack.png");
    gBlackhole = loadImage("resources/imgs/blackhole.png");
	gHealthbar = loadImage("resources/imgs/healthbar.png");

	int scrollOffset = 0;
	int rem = 0;
	double xDeltav = 0.0;
	double yDeltav = 0.0;
	double x2DeltaV = 0.0;
	double y2DeltaV = 0.0;
	bool create;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	int frames = 0;
	int frameCount = 0;

	SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	Uint32 fpsLasttime = SDL_GetTicks();
	Uint32 fpsCurtime = 0;
	Uint32 moveLasttime = SDL_GetTicks();
	double timestep = 0;
	SDL_Rect attackRect = {0, 0, 60, 10};
	SDL_Rect attackRect2 = {0, 0, 60, 10};
	//SDL_Rect attackCam = {SCREEN_WIDTH+80, SCREEN_HEIGHT/2+51/2, 80, 20};
	SDL_Rect blackholeRect = {0, 0, 300, 300};
	SDL_Rect blackholeCam = {SCREEN_WIDTH,SCREEN_HEIGHT/2, 300, 300};
	Player ply(10, loadImage("resources/imgs/starman.png"), 1, gRenderer);
	Player ply2(10, loadImage("resources/imgs/starman_blue.png"), 1, gRenderer);

	Enemy emy(10, loadImage("resources/imgs/faxanaduitis.png"), 1,&ply.hit, 'f');
	emy.setPosition(860, 0);
	emy.setVelocity(0, 50);

	SDL_Rect healthRect = {0, 0, 177, 33};
	SDL_Rect healthCam = {30, 30, 177, 33};
	HyperStar stars(loadImage("resources/imgs/star4.png"),&ply);

	Magnetar mag(&ply, loadImage("resources/imgs/Magnetars.png"));

	//~ Removed for demo
	// AlcoholCloud ac(&ply, &emy, loadImage("resources/imgs/Alcohol_Cloud.png"), loadImage("resources/imgs/Alcohol_Cloud_Flare_Up.png"), &ply.hit);
	double ACCEL = ply.GetMove();

	ply.HealthBar(&healthRect);//needed healthbar in player
	Shield protect(loadImage("resources/imgs/shield_powerup.png"), loadImage("resources/imgs/shield.png"), &ply);

	mag.Multiplayer(&ply2);

	//the beginning/default image and attack box
	ply.hit.setAttack(gAttack,&attackRect);
	ply2.hit.setAttack(gAttack, &attackRect2);
	SDL_Event e;
	bool gameOn = true;
	bool up = true;
	bool credits = true;
    double emyDelta = 1;
    int connected = 0;

	while(gameOn)
	{
		if(selection == 3 && !connected){
			connected = client->Connect();
		}

		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameOn = false;
				credits = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					gameOn = false;
					credits = false;
				}
				if (e.key.keysym.sym == SDLK_q)
				{
					gameOn = false;
				}
			}
			if(up == false && e.type == SDL_KEYUP && e.key.repeat == 0)
			{
				if(e.key.keysym.sym == SDLK_SPACE)
				{
					up = true;
				}
			}
		}

		ACCEL = ply.GetMove();
		timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;
		xDeltav = 0.0;
		yDeltav = 0.0;
		x2DeltaV = 0.0;
		y2DeltaV = 0.0;

		// WASD movement
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_A])
			xDeltav -= (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_D])
			xDeltav += (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_W])
			yDeltav -= (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_S])
			yDeltav += (ACCEL * timestep);

		if (emy.getEnemyCam().y + emy.getEnemyCam().h == SCREEN_HEIGHT)
		{
			emyDelta = -1;
			emy.setVelocity(0, -10);
		}
		if (emy.getEnemyCam().y == 0)
		{
			emyDelta = 1;
			emy.setVelocity(0, 10);
		}

		SDL_Rect pRect = ply.getPlayerRect();
		SDL_Rect pCam = ply.getPlayerCam();
		SDL_Rect pRect2 = ply2.getPlayerRect();
		SDL_Rect pCam2 = ply.getPlayerCam();
		SDL_Rect transfer;

		SDL_Rect eRect = emy.getEnemyRect();
		SDL_Rect eCam = emy.getEnemyCam();

		moveLasttime = SDL_GetTicks();

		// Scrolling background
		++scrollOffset;
		if (scrollOffset > bgRect.w)
		{
			scrollOffset = 0;
		}

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		rem = scrollOffset % SCREEN_WIDTH;
		bgRect.x = -rem;


		SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);
		bgRect.x += SCREEN_WIDTH;
		SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);

		frames += 1 % 1000000000;

		ply.animate(frames);
		if(connected){
			ply2.animate(frames);
		}

		emy.animate(frames);


		// Since game levels progress from L to R, no need for sprite to flip
		// Code for flipping remains here if theres a change of plan

		// Flip if facing other direction
		/*if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_NONE;
		else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			flip = SDL_FLIP_HORIZONTAL;*/

		pRect = ply.getPlayerRect();
		pCam = ply.getPlayerCam();

		pRect2 = ply2.getPlayerRect();

        Uint32 currTime = SDL_GetTicks();
		
        if(currTime>=6000)
		{
            //std::cout << currTime % 3000 << std::endl;
			if((currTime % 10000 <= 50 && !mag.Seen()) ||mag.Seen())
			{

				mag.Render();
			}
			if(currTime%3000<=20)
			{
				stars.addStar();
			}
			if(currTime%5000<=20)
			{
				protect.NewItem();
			}
		}
        if(currTime >= 5000)
        {
            int bFrames;
            if(currTime % 5000 == 0)
            {
                SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
                bFrames = 0;
                //blackhole vacuum(gRenderer,gBlackhole,&blackholeRect,blackholeCam);
            }
            else
            {
                bFrames++;

                if (bFrames / 12 > 5)
                    bFrames = 0;

                blackholeRect.x = (bFrames / 12) * 300;
                blackholeCam.x = blackholeCam.x - 1;
                SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);

                if(blackholeCam.x < SCREEN_WIDTH && blackholeCam.x > 0)
                {
                    if(blackholeCam.x + 150 > pCam.x)
                    {
                        if(blackholeCam.y + 150 > pCam.y)
                        {
                            yDeltav = yDeltav + 20;
                        }
                        if(blackholeCam.y + 150 < pCam.y)
                        {
                            yDeltav = yDeltav - 20;
                        }
                        xDeltav = xDeltav + 20;
                    }
                    else if(blackholeCam.x + 150 < pCam.x)
                    {
                        if(blackholeCam.y + 150 > pCam.y)
                        {
                            yDeltav = yDeltav + 20;
                        }
                        if(blackholeCam.y + 150 < pCam.y)
                        {
                            yDeltav = yDeltav - 20;
                        }
                        xDeltav = xDeltav - 20;
                    }
                }
            }

            if(blackholeCam.x == -300)
            {
                blackholeCam = {SCREEN_WIDTH,rand() % (SCREEN_HEIGHT-300), 300, 300};
                bFrames = 0;
            }
        }

		ply.move(xDeltav, yDeltav, timestep);
		bool collision = ply.checkEnemyCollision(&emy, timestep);

		if (collision)
		{
			//ply.LostHealth(1);
			if (healthRect.x == 1770)
			{
				return playCredits(&openGL);
			}
			else
			{
				healthRect.x += 177;
			}
		}

		if(healthRect.x >= 1598)//will now play credits when health is gone
		{
			return playCredits(&openGL);
		}
		emy.move(0, emyDelta, timestep);
		emy.checkPlayerCollision(&ply, timestep);
		/*
		collision = emy.checkPlayerCollision(&ply, timestep);
		if (collision)
		{
			//Minor enemies should be destroyed in event of collision
		}
		*/
		pCam = ply.getPlayerCam();

		if(connected){
			transfer = client->Communicate(pCam);
		}
		pCam2.x = transfer.x;
		pCam2.y = transfer.y;

		eCam = emy.getEnemyCam();

        //attack button

		if(keyState[SDL_SCANCODE_SPACE] && up == true)
		{
			up = false;
			ply.hit.addAttack(pCam.x + 240,pCam.y + 51/2);

			//play fire sound effect
			mus.fireSound();
		}
		//lets the attack move across the screen
		ply.hit.renderAttack(timestep);
		SDL_RenderCopyEx(gRenderer, ply.getPlayerSheet(), &pRect, &pCam, 0.0, nullptr, flip);
		protect.Render();
		SDL_RenderCopyEx(gRenderer, emy.getEnemySheet(), &eRect, &eCam, 0.0, nullptr, flip);


		//~ removed for demo
		// if (ac.getDelay() == 0)
		// {
		// 	ac.setDelay((rand() % 3000) + 5000);
		// }

		// if (currTime >= ac.getDelay())
		// {
		// 	if (!ac.Seen())
		// 	{
		// 		ac.setYPosition(rand() % 421);
		// 	}

		// 	ac.Render();
		// }

		stars.Render(timestep);

		if(Multiplayer){
			SDL_RenderCopyEx(gRenderer, ply2.getPlayerSheet(), &pRect2, &pCam2, 0.0, nullptr, flip);
		}

		SDL_RenderCopy(gRenderer, gHealthbar, &healthRect, &healthCam);

		SDL_RenderPresent(gRenderer);

	}
	if (credits)
	{
		return playCredits(&openGL);
	}

	// When the main loop ends, we can assume it is time for openGL to die
	// And yeah, should be in the close function (but it isn't because I instantiate openGL on the stack)
	openGL.Close();

	close();
	//return credits ? playCredits(&openGL) : close();
}
