#include "AlcoholCloud.h"
#include <iostream>

AlcoholCloud::AlcoholCloud(Player* p, SDL_Texture* i):ply(p), sprite(i)
{
	gRenderer = ply->getRend();
	spriteBox = {0, 0, 500, 300};
	
	//Get a pointer to the player's camBox
	playerCam = ply->getPlayerCamLoc();
	alcoholCam = {1530, 150, 500, 300};
	surroundingAlcoholCam = {1280, 0, 1000, 600};
	onScreen = false;
	frame = 0;
}

void AlcoholCloud::Render()
{
	//animation
	if((frame/10) >= 4)
	{
		frame = 0;
	}
	
	spriteBox.x = ((frame / 10) % 4) * spriteBox.w;
	frame++;
	
	//Places object to screen
	SDL_RenderCopy(gRenderer, sprite, &spriteBox, &alcoholCam);
	
	//bool inter = SDL_HasIntersection(&camBox,playerCam);
	
	alcoholCam.x = alcoholCam.x - 1;
	surroundingAlcoholCam.x = surroundingAlcoholCam.x - 1;
	
	//Used to determine how long to call the alcohol cloud for
	if(surroundingAlcoholCam.x + surroundingAlcoholCam.w > 0)
	{
		onScreen = true;
	}
	else
	{
		onScreen = false;
		surroundingAlcoholCam.x = 1280;
		surroundingAlcoholCam.y = 0;
		alcoholCam.x = 1530;
		alcoholCam.y = 150;
		delay = 0;
	}
}

//Whether or not the alcohol cloud is still on the screen
bool AlcoholCloud::Seen()
{
	return onScreen;
}

//Set the delay of when the next alcohol cloud will be rendered to the screen
void AlcoholCloud::setDelay(int time)
{
	if (delay == 0)
	{
		delay = time;
	}
}

//Get the delay of the next alcohol cloud
int AlcoholCloud::getDelay()
{
	return delay;
}

void AlcoholCloud::setYPosition(int y)
{
	surroundingAlcoholCam.y = alcoholCam.y - 150;
	alcoholCam.y = y;
}
