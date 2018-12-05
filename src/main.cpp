// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdlib>
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
#include "VirtualPeacefulKing.h"
#include "GameOver.h"

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

//Boolean value to control when the boss got rendered
bool showTime = false;
bool bossOn = false;
//The variables that will be used by timer
double timePassed = 0.0;
double timeLimit = 4.0;//Subjust to be changed


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
OpenGLRenderer* gRenderer = nullptr;
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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

int close()
{
	mus.close();

	//~ SDL_DestroyRenderer(gRenderer);
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

	Uint32 startTime = SDL_GetTicks();
	double timeDelta = 0;

	//std::cout << creditNames[index].c_str() << std::endl;
	RenderObject *currentCreditImage = new RenderObject(
		0, 0, 1, openGL->allBufferAttributes[creditNames[index].c_str()]
	);
	openGL->AppendRenderObject(currentCreditImage);

	openGL->Display();

	// Magic number length
	while (index < 10 && creditsContinue)
	{
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

		// Wait 3 seconds
		if (timeDelta >= 3000)
		{
			index++;
			// Magic number length
			if (index > 9)
			{
				break;
			}

			const char *currentCreditName = creditNames[index].c_str();

			startTime = SDL_GetTicks();
			timeDelta = 0;

			// Kill image
			openGL->RemoveRenderObject(currentCreditImage->index);

			// Get new
			//std::cout << currentCreditName << std::endl;
			currentCreditImage = new RenderObject(
				0, 0, 1, openGL->allBufferAttributes[currentCreditName]
			);
			openGL->AppendRenderObject(currentCreditImage);

			openGL->Display();
		}
		else
		{
			timeDelta = SDL_GetTicks() - startTime;
		}
	}

	if (currentCreditImage) {
		// Kill image
		openGL->RemoveRenderObject(currentCreditImage->index);
	}

	//close();
	return 0;
}

// Test main
int main(int argc, char* argv[]) {
	// Multiplayer stuff
	bool multiplayer = false;
	int connected = 0;
	bool imPlayer2 = false;
	printf("%d",argc);
	
	string ip;
	std::ifstream myfile ("config.txt");
	if (myfile.is_open())
	{
		while ( getline (myfile,ip) )
		{
			//cout << line << '\n';
		}
    	myfile.close();
    }

    if(ip != "localhost"){
		imPlayer2 = true;
    }
	client = new ClientInterface(ip);

	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	// Music
	mus = music();
	mus.init();
	mus.playMusic();

	// OpenGL init
	OpenGLRenderer openGL = OpenGLRenderer(gWindow);

	// Menu
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
			multiplayer = true;
		}
		//cout << "SELECTION " << selection << endl;
	}

	// Now that we are ready to start the game, clean the openGLRenderer
	openGL.TabulaRasa();

	// FPS params
	Uint32 fpsLasttime = SDL_GetTicks();
	Uint32 fpsCurtime = 0;
	Uint32 moveLasttime = SDL_GetTicks();
	double timestep = 0;

	int rem = 0;
	double xDeltav = 0.0;
	double yDeltav = 0.0;
	double x2DeltaV = 0.0;
	double y2DeltaV = 0.0;
	bool create;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	int frames = 0;
	int frameCount = 0;

	// THE BACKGROUND
	RenderObject *background1 = new RenderObject(
		0, 0, -1, openGL.allBufferAttributes["resources/imgs/space_2_background.png"]
	);

	RenderObject *background2 = new RenderObject(
		SCREEN_WIDTH, 0, -1, openGL.allBufferAttributes["resources/imgs/space_2_background.png"]
	);

	// ADD THE BACKGROUND IMAGES TO THE QUEUE
	openGL.AppendRenderObject(background1);
	openGL.AppendRenderObject(background2);

	// THE PLAYER(S)
	char player1Texture[] = "resources/imgs/starman.png";
	char player2Texture[] = "resources/imgs/starman_blue.png";
	Player ply(10, player1Texture, 1, &openGL, true);
	Player ply2(10, player2Texture, 1, &openGL, multiplayer);
	double ACCEL = ply.GetMove();

	// PLAYER ATTACKS
	SDL_Rect attackRect = {0, 0, 60, 10};
	SDL_Rect attackRect2 = {0, 0, 60, 10};
	ply.hit.setAttack(&attackRect);
	if (multiplayer)
	{
		ply2.hit.setAttack(&attackRect2);
		ply2.render->flipped = true;
	}

	// FAXANDUITIS
	Enemy emy(&openGL, &ply, 1, &ply.hit, 'f', &timestep);
	double emyDelta = 1;

	// THE KING
	//Our king appears!!!!!
	VirtualPeacefulKing king(&openGL, 100, 2, 4);
	double kingDelta = 1;

	// First throw the king into the void
    //The player will not be able to see the boss until some time has been passed
    king.setPosition(11000, 0,showTime);
    king.setVelocity(0, 50);
	ply.setAttackColSound(&mus);
	// HYPERSTAR
	HyperStar stars(&ply, &mus);

	// BLACKHOLE
	blackhole enemyBlackhole(&ply);
	bool blackholeHit = false;

	// MAGNETAR
	Magnetar mag(&ply);

	if (multiplayer)
	{
		mag.Multiplayer(&ply2);
	}

	// ALCOHOL CLOUD
	AlcoholCloud ac(&ply, &emy, &ply.hit);

	// SHIELD
	Shield protect(&ply);

	// PLAYER HEALTHBAR
	// THIS OCCURS LATER FOR A REASON
	RenderObject* healthBar = new RenderObject(
		30, 30, 1, openGL.allBufferAttributes["resources/imgs/healthbar.png"]
	);
	ply.HealthBar(healthBar);
	openGL.AppendRenderObject(healthBar);

	// LOOP STUFF
	SDL_Event e;
	bool gameOn = true;
    bool gameOver = false;
	bool up = true;
	bool credits = true;
	bool attacked = false;

    //Set up the timer
    clock_t startTimeForBoss = clock();
    
	while (gameOn)
	{
		if(multiplayer && !connected) {
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
			if (up == false && e.type == SDL_KEYUP && e.key.repeat == 0)
			{
				if(e.key.keysym.sym == SDLK_SPACE)
				{
					up = true;
				}
			}
		}

		// Modify ACCEL
		ACCEL = ply.GetMove();

		// Increment timestep
		timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;

		// Physics stuff
		xDeltav = 0.0;
		yDeltav = 0.0;
		x2DeltaV = 0.0;
		y2DeltaV = 0.0;

		// WASD movement
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_A])
			xDeltav -= (abs(ACCEL) * timestep);
		if (keyState[SDL_SCANCODE_D])
			xDeltav += (abs(ACCEL) * timestep);
		if (keyState[SDL_SCANCODE_W])
			yDeltav -= (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_S])
			yDeltav += (ACCEL * timestep);

		// RECTS I GUESS
		SDL_Rect pRect = ply.getPlayerRect();
		SDL_Rect pCam = ply.getPlayerCam();
		SDL_Rect pRect2 = ply2.getPlayerRect();
		SDL_Rect pCam2 = ply.getPlayerCam();
		SDL_Rect transfer;
		SDL_Rect kRect = king.getRect();
		SDL_Rect kCam = king.getCamera();

		moveLasttime = SDL_GetTicks();

		// ANIMATE THE PLAYER(S)
		if(!imPlayer2){
			ply.animate(frames);
			if (connected){
				ply2.animate(frames);
			}
		}
		else{
			ply2.animate(frames);
			if (connected){
				ply.animate(frames);
			}
		}

		// Animate king (test)
		//boundary check for king
		if (king.getCamera().y + king.getCamera().h == SCREEN_HEIGHT)
		{
			kingDelta = -1;
			king.setVelocity(0, -10);
		}
		if (king.getCamera().y == 0)
		{
			kingDelta = 1;
			king.setVelocity(0, 10);
		}
        
        //The boss for only get rendered after 20 seconds
        timePassed = (clock() - startTimeForBoss)/ CLOCKS_PER_SEC;
        if (timePassed >= timeLimit)
        {
            showTime = true;
        }
        
        if (showTime)
        {
            if (!bossOn)
            {
                //Put our king back online
                king.setPosition(1100, 0,showTime);
                bossOn = true;
            }
            
            king.animate(frames);
        }
		
        std::cout << timePassed << std::endl;
		// Since game levels progress from L to R, no need for sprite to flip
		// Code for flipping remains here if theres a change of plan

		// Flip if facing other direction
		/*if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_NONE;
		else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			flip = SDL_FLIP_HORIZONTAL;*/

		// CURRENT TIME
		Uint32 currTime = SDL_GetTicks();

		// TIME BASED ACTIONS: TO BE REVISED
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
			//int bFrames;
			if((currTime % 5000 < 50 && !enemyBlackhole.seen()) || enemyBlackhole.seen())
			{
				//SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
				//bFrames = 0;
				//blackhole vacuum(gRenderer,gBlackhole,&blackholeRect,blackholeCam);
        
				blackholeHit = enemyBlackhole.showBlackhole(xDeltav, yDeltav, timestep);
				if(blackholeHit)
				{
					gameOn = false;
					gameOver = true;
				}
			}
		}

		if(!imPlayer2){
			ply.move(xDeltav, yDeltav, timestep);
		}
		else{
			ply2.move(xDeltav, yDeltav, timestep);
		}
		ply.move(xDeltav, yDeltav, timestep);
		ply.checkInvincibility(moveLasttime);



        
        
        
        //Check for king's collision
        if (SDL_HasIntersection(king.getCameraLoc(), ply.getPlayerCamLoc()) || king.checkRectCollision(king.getCameraLoc(), ply.getPlayerCamLoc()))
        {
            
            if (ply.getPlayerCam().y < 0 || (ply.getPlayerCam().y + 50 > SCREEN_HEIGHT))
            {
                ply.getPlayerCamLoc()->y -= yDeltav;
            }
            
            if (ply.getPlayerCam().x < 0 || (ply.getPlayerCamLoc()->x + 240 > SCREEN_WIDTH))
            {
                ply.getPlayerCamLoc()->x -= xDeltav;
            }
            
            
            ply.LostHealth(1);
            ply.damage(1);
        }

		if (emy.Exists())
		{
			bool collision = ply.checkEnemyCollision(&emy, timestep);

			emy.Render();

			if (collision && emy.GetHealth() > 0)
			{
				ply.LostHealth(1);
				ply.damage(1);
			}
		}
		else
		{
			if (emy.getNextSpawn() == 0)
			{
				emy.setNextSpawn((rand() % 5001) + 5000 + currTime);
			}

			if (currTime >= emy.getNextSpawn())
			{
				emy.Spawn();
			}
		}

		//~ if(healthRect.x >= 1598)//will now play credits when health is gone
		if (healthBar->currentBufferID == healthBar->bufferAttributes.bufferIDEnd)
		{
			//~ return playCredits();
			gameOn = false;
			gameOver = true;
		}

		king.move(0, kingDelta, timestep,showTime);

		/*
		collision = emy.checkPlayerCollision(&ply, timestep);
		if (collision)
		{
			//Minor enemies should be destroyed in event of collision
		}
		*/
		int sendAttack = 0;
		if(keyState[SDL_SCANCODE_SPACE] && up == true){
			sendAttack = 1;
		}

		if(connected){
			if(imPlayer2){
				pCam2 = ply2.getPlayerCam();
				transfer = client->Communicate(pCam2, sendAttack);
				pCam.x = transfer.x;
				pCam.y = transfer.y;
				attacked = transfer.w;
				ply.render->ChangeCoordinates(pCam.x, pCam.y, ply.render->z);
			}
			else{
				pCam = ply.getPlayerCam();
				transfer = client->Communicate(pCam, sendAttack);
				pCam2.x = transfer.x;
				pCam2.y = transfer.y;
				attacked = transfer.w;
				ply2.render->ChangeCoordinates(pCam2.x, pCam2.y, ply2.render->z);
			}
		}
		kCam = king.getCamera();

		//attack button
		if(keyState[SDL_SCANCODE_SPACE] && up == true)
		{
			up = false;
			if(!imPlayer2){
				ply.hit.addAttack(pCam.x + 240, pCam.y + 51/2);
			}
			else{
				ply2.hit.addAttack(pCam2.x, pCam2.y + 51/2);
			}
			//play fire sound effect
			mus.fireSound();
		}
		//lets the attack move across the screen
		if(attacked){
			if(!imPlayer2){
				ply2.hit.addAttack(pCam2.x, pCam2.y + 51/2);
			}
			else{
				ply.hit.addAttack(pCam.x, pCam.y + 51/2);
			}
		}

		ply.hit.renderAttack(timestep, 0);
		ply.hit.hitIntersect(&pCam2);
		ply2.hit.renderAttack(timestep, 1);
		ply2.hit.hitIntersect(&pCam);
		protect.Render();

		//~ ALCOHOL CLOUD STUFF
		if (ac.getDelay() == 0)
		{
		 	ac.setDelay((rand() % 3000) + 5000);
		}

		if (currTime >= ac.getDelay())
		{
		 	if (!ac.Seen())
		 	{
		 		ac.setYPosition(rand() % 421);
		 	}

		 	ac.Render();
		}

		if (healthBar->currentBufferID == healthBar->bufferAttributes.bufferIDEnd)
		{
			gameOn = false;
			gameOver = true;
		}

		// MODIFY STARS
		stars.Render(timestep);

		// THE BG THE BG
		// Move the background
		background1->ChangeCoordinates(
			background1->x - 1,
			background1->y,
			background1->z
		);
		background2->ChangeCoordinates(
			background2->x - 1,
			background2->y,
			background2->z
		);

		// Reset the background as necessary
		if (background1->x < -SCREEN_WIDTH)
		{
			background1->ChangeCoordinates(0, background1->y, background1->z);
			background2->ChangeCoordinates(SCREEN_WIDTH, background2->y, background2->z);
		}

		// Displays stuff
		openGL.Display();
	}

	// Contrivance for memory leak
	// (In this case, multiplayer render is not freed because not in queue)
	//~ if (!multiplayer)
	//~ {
		//~ delete ply2.render;
	//~ }
    if (gameOver)
	{	
		openGL.TabulaRasa();
		GameOver screen = GameOver(&openGL);
		int selection = screen.runScreen();
		if (selection == 0 || selection == 2)
		{
			openGL.Close();
			close();
			return 0;
		}
		if (selection == 1)
		{
			playCredits(&openGL);
			selection = screen.runScreen();	
		}
	}
	if (credits)
	{
		playCredits(&openGL);
	}

	openGL.Close();
	close();
	return -1;
}
