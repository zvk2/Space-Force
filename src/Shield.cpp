#include "Shield.h"
Shield::Shield(Player* main): ply(main)
{
	//~ std::string initTexture = "resources/imgs/shield_powerup.png";
	//~ itemTexture = initTexture.c_str();

	//~ initTexture = "resources/imgs/shield.png";
	//~ protectTexture = initTexture.c_str();

	openGL = ply->getRend();
	plyCam = ply->getPlayerCamLoc();
	int size = 50;
	itemLoc = {1280,720,size,size};
	itemIm = {0,0,100,100};
	size = 300;
	protectLoc = {-size,-size,size,size};;
	protectIm = {0,0,300,300};;
	itemScreen = false;
	screen = false;
	hits = 0;
	addStrength = 2;
	ply->shieldLocation(&protectLoc, &hits);
	ply->hasShield(false);

	// Only one render for item and one for protect
	renderItem = new RenderObject(
		itemLoc.x, itemLoc.y, 1, openGL->allBufferAttributes["resources/imgs/shield_powerup.png"]
	);
	openGL->AppendRenderObject(renderItem);

	renderProtect = new RenderObject(
		protectLoc.x, protectLoc.y, 1, openGL->allBufferAttributes["resources/imgs/shield.png"]
	);
	openGL->AppendRenderObject(renderProtect);
}
void Shield::Render()
{

	if(itemScreen)
	{
		if(!screen)
		{
			int range = 721 - itemLoc.h;
			itemLoc.y = rand()%range;
			screen = true;
		}
		//~ SDL_RenderCopy(gRenderer, item, &itemIm, &itemLoc.;
		bool intersect = SDL_HasIntersection(&itemLoc, plyCam);
		itemLoc.x = itemLoc.x - 1;
		if(itemLoc.x < -itemLoc.w || intersect)
		{
			itemScreen = false;
			screen = false;
			itemLoc.x = 1280;
			if(intersect)
			{
				ply->hasShield(true);
				hits = hits + addStrength;
			}
		}
		renderItem->ChangeCoordinates(
			itemLoc.x,
			itemLoc.y,
			renderItem->z
		);
	}
	if(hits <= 0)
	{
		hits = 0;
		ply->hasShield(false);

		renderProtect->ChangeCoordinates(
			2000,
			2000,
			renderProtect->z
		);
	}
	if(hits > 0)
	{
		RenderPower();
	}

}
void Shield::RenderPower()
{
	protectLoc.x = plyCam->x - 30;
	protectLoc.y = plyCam->y - 125;
	//~ SDL_RenderCopy(gRenderer, protect, &protectIm, &protectLoc);
	renderProtect->ChangeCoordinates(
		protectLoc.x,
		protectLoc.y,
		renderProtect->z
	);
	renderProtect->IterateFrame();
}
void Shield::Damage(int hitsTaken)
{
	hits = hits - hitsTaken;
	if(hits <= 0)
	{
		hits = 0;
		ply->hasShield(false);
		//~ renderProtect->ChangeCoordinates(
			//~ 2000,
			//~ 2000,
			//~ renderProtect->z
		//~ );
	}
}
void Shield::NewItem()
{
	itemScreen = true;
}
