
#include "Health.h"

Health::Health(SDL_Texture* imItem, Player* main): item(imItem), ply(main)
{
	gRenderer = ply->getRend();
	plyCam = ply ->getPlayerCamLoc();
	int size = 50;
	itemLoc = {1280,720,size,size};
	itemIm = {0,0,100,100};
	size = 300;
	healthLoc = {-size,-size,size,size};;
	healthIm = {0,0,300,300};;
	itemScreen = false;
	screen = false;
	ply -> healthLocation(&healthLoc);
}

void Health::Render()
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
				ply->healthIncrease(true);
			}
			
		}
		
	}
}

void Health::NewItem()
{
	itemScreen = true;
}
