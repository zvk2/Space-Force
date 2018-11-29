#include "Shield.h"
Shield::Shield(SDL_Texture* imItem, SDL_Texture* power, Player* main): item(imItem), protect(power),ply(main)
{
	gRenderer = ply->getRend();
	plyCam = ply ->getPlayerCamLoc();
	int size = 100;
	itemLoc = {1280,720,size,size};
	itemIm = {0,0,size,size};
	size = 300;
	protectLoc = {1280,720,size,size};;
	protectIm = {0,0,size,size};;
	itemScreen = false;
	screen = false;
	hits = 0;
	addStrength = 2;
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
	//play.x - 30
	SDL_RenderCopy(gRenderer, protect, &itemIm, &itemLoc);
}
void Shield::newItem()
{
	itemScreen = true;
}