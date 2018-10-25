#include "Magnetar.h"
Magnetar::Magnetar(Player* main, SDL_Texture* gIm):ply(main), im(gIm)
{
	gRenderer = ply->getRend();
	collBox = {0,0,1280,721};
	playerCam = ply->getPlayerCamLoc();
	camBox = {1280,0,1280,720};
	onScreen = false;
	frame = 0;
}
void Magnetar::Render()
{	
	if(frame >= 9)
		frame = 0;
	collBox.y = (frame/3)*721;
	frame++;
	SDL_RenderCopy(gRenderer, im, &collBox, &camBox);
	double ACCEL = ply->getACCEL();
	bool inter = SDL_HasIntersection(&camBox,playerCam);
	if ((inter && ACCEL > 0) || (!inter && ACCEL < 0))
	{
		ply->changeAccel(-ACCEL);
	}
	camBox.x = camBox.x - 1;
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
bool Magnetar::seen()
{
	return onScreen;
}