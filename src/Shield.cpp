#include "Shield.h"
Shield::Shield(SDL_Texture* imItem, SDL_Texture* power, Player* main): item(imItem), protect(power),ply(main)
{
	gRenderer = ply->getRend();
	plyCam = ply ->getPlayerCamLoc();
	int size = 50;
	itemLoc = {1280,720,size,size};
	itemIm = {0,0,100,100};
	size = 300;
	protectLoc = {1280,720,size,size};;
	protectIm = {0,0,300,300};;
	itemScreen = false;
	screen = false;
	hits = 0;
	addStrength = 2;
	ply -> shieldLocation(&protectLoc);
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
		SDL_RenderCopy(gRenderer, item, &itemIm, &itemLoc);
		bool intersect = SDL_HasIntersection(&itemLoc,plyCam);
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
	SDL_RenderCopy(gRenderer, protect, &protectIm, &protectLoc);
	
}
void Shield::NewItem()
{
	itemScreen = true;
}