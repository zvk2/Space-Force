#include "Magnetar.h"
#include <iostream>
Magnetar::Magnetar(Player* main, SDL_Texture* gIm):ply(main), im(gIm)
{
	gRenderer = ply->getRend();
	collBox = {0,0,1280,721};
	
	//gets a pointer to the player's camBox
	playerCam = ply->getPlayerCamLoc();
	camBox = {1280,100,1280,520};
	onScreen = false;
	frame = 0;
}
void Magnetar::Render()
{	
	//animation
	if(frame >= 9)
	{
		frame = 0;
	}
	collBox.y = (frame/3)*721;
	frame++;
	
	//places object to screen
	SDL_RenderCopy(gRenderer, im, &collBox, &camBox);
	//gets players current ACCEL
	double ACCEL = ply->GetMove();
	bool inter = SDL_HasIntersection(&camBox,playerCam);
	
	//if player has intersected with the object and move is postive it
	//will make it negative to inverse controls
	//but if the intersection breaks will make ACCEL postive again
	if ((inter && ACCEL > 0) || (!inter && ACCEL < 0))
	{
		ply->ChangeMove(-ACCEL);
	}
	camBox.x = camBox.x - 1;
	
	//Used to determine how long to call Magnetar for
	if(camBox.x + 1280> 0)
	{
		onScreen = true;
	}
	else
	{
		onScreen = false;
		camBox.x = 1280;
	}
}
//whether or not Magnetar is still on screen
bool Magnetar::Seen()
{
	return onScreen;
}