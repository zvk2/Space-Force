#include "Magnetar.h"
#include <iostream>
// NOTE AT THE MOMENT MAGNETAR JUST KEEPS SCROLLING
Magnetar::Magnetar(Player* main):ply(main)
{
	openGL = ply->getRend();
	collBox = {0,0,1280,721};

	//gets a pointer to the player's camBox
	playerCam = ply->getPlayerCamLoc();
	camBox = {1280,0,1280,721};
	onScreen = false;
	frame = 0;

	// There can be only one magnetar, when it isn't in use it will wait off screen
	render = new RenderObject(
		camBox.x, camBox.y, 0.5, openGL->allBufferAttributes["resources/imgs/Magnetars.png"]
	);
	
	textH = 78;
	textW = 571;
	textBox = {0,0,textW,textH};
	textScreen = {(1280/2)-textW/2,720+textH,textW,textH};
	textRender = new RenderObject(textScreen.x, textScreen.y,1,openGL->allBufferAttributes["resources/imgs/Mag_Font2.png"]);

	openGL->AppendRenderObject(render);
	openGL->AppendRenderObject(textRender);
}

void Magnetar::Render()
{
	//animation
	//~ if(frame >= 9)
	//~ {
		//~ frame = 0;
	//~ }
	//~ collBox.y = (frame/3)*721;
	//~ frame++;
	render->IterateFrame();

	//places object to screen
	//~ SDL_RenderCopy(gRenderer, im, &collBox, &camBox);
	//gets players current ACCEL
	double ACCEL = ply->GetMove();
	bool inter = SDL_HasIntersection(&camBox, playerCam);

	double ACCEL2;
	bool inter2;
	if(multiplayer)
	{
		ACCEL2 = ply2->GetMove();
		inter2 = SDL_HasIntersection(&camBox, player2Cam);

		if((inter2 && ACCEL2 > 0) || (!inter2 && ACCEL < 0))
		{
			ply2->ChangeMove(-ACCEL2);
		}
	}
	//if player has intersected with the object and move is postive it
	//will make it negative to inverse controls
	//but if the intersection breaks will make ACCEL postive again
	if ((inter && ACCEL > 0) || (!inter && ACCEL < 0))
	{
		ply->ChangeMove(-ACCEL);
	}
	camBox.x = camBox.x - 1;
	if(inter && ACCEL > 0)
	{
		textRender->ChangeCoordinates(
		textScreen.x,
		0,
		textRender->z);
	}
	else if(!inter && ACCEL < 0)
	{
		textRender->ChangeCoordinates(
		textScreen.x,
		textScreen.y,
		textRender->z);
	}

	//Used to determine how long to call Magnetar for
	if(camBox.x + SCREEN_WIDTH > 0)
	{
		onScreen = true;
	}
	else
	{
		onScreen = false;
		camBox.x = SCREEN_WIDTH;
	}

	render->ChangeCoordinates(
		camBox.x,
		camBox.y,
		render->z
	);
}
//whether or not Magnetar is still on screen
bool Magnetar::Seen()
{
	return onScreen;
}

void Magnetar::Multiplayer(Player* player2)
{
	ply2 = player2;
	player2Cam = ply2->getPlayerCamLoc();
	multiplayer = true;
}
