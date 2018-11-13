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
	if((frame / 10) >= 4)
	{
		frame = 0;
	}
	
	spriteBox.x = ((frame / 10) % 4) * spriteBox.w;
	frame++;
	
	//Places object to screen
	SDL_RenderCopy(gRenderer, sprite, &spriteBox, &alcoholCam);
	SDL_Rect result;
	
	if (SDL_IntersectRect(&surroundingAlcoholCam, playerCam, &result))
	{
		if (SDL_HasIntersection(&alcoholCam, playerCam))
		{
			ply->ChangeMaxVelocity((double) (rand() % 291) + 10);
		}
		else
		{
			int distance_x, distance_y, closest_x, closest_y, minVelocity;
			double percent_distance;
			
			if ((result.x + result.w) <= alcoholCam.x)
			{
				distance_x = (result.x + result.w - 1) - alcoholCam.x;
				closest_x = alcoholCam.x;
			}
			else if (result.x >= (alcoholCam.x + alcoholCam.w))
			{
				distance_x = result.x - (alcoholCam.x + alcoholCam.w - 1);
				closest_x = (alcoholCam.x + alcoholCam.w - 1);
			}
			else
			{
				distance_x = 10000;
			}

			if ((result.y + result.h) <= alcoholCam.y)
			{
				distance_y = (result.y + result.h - 1) - alcoholCam.y;
				closest_y = alcoholCam.y;
			}
			else if (result.y >= (alcoholCam.y + alcoholCam.h))
			{
				distance_y = result.y - (alcoholCam.y + alcoholCam.h - 1);
				closest_y = (alcoholCam.y + alcoholCam.h - 1);
			}
			else
			{
				distance_y = 10000;
			}
			
			if ((distance_x != 10000) && (distance_y != 10000))
			{
				double slope, alcoholSlope;
				int point_x, point_y;
				
				alcoholSlope = (alcoholCam.y - surroundingAlcoholCam.y) / (alcoholCam.x - surroundingAlcoholCam.x);
				slope = (alcoholCam.y - (result.y + result.h - 1)) / (alcoholCam.x - (result.x + result.w - 1));
				
				//Positive slope
				if (slope > 0)
				{	
					if (slope < alcoholSlope)
					{
						point_x = surroundingAlcoholCam.x - 1;
						point_y = slope * (surroundingAlcoholCam.x - 1);
					}
					else if (slope > alcoholSlope)
					{
						point_x = (surroundingAlcoholCam.y - 1) / slope;
						point_y = surroundingAlcoholCam.y - 1;
					}
					else
					{
						if (distance_x < 0)
						{
							point_x = surroundingAlcoholCam.x - 1;
							point_y = surroundingAlcoholCam.y - 1;
						}
						else
						{
							point_x = surroundingAlcoholCam.x + surroundingAlcoholCam.w;
							point_y = surroundingAlcoholCam.y + surroundingAlcoholCam.h;
						}
					}
				}
				//Negative slope
				else
				{
					if (slope < -alcoholSlope)
					{
						point_x = (surroundingAlcoholCam.y - 1) / slope;
						point_y = surroundingAlcoholCam.y - 1;
					}
					else if (slope > -alcoholSlope)
					{
						point_x = surroundingAlcoholCam.x - 1;
						point_y = slope * (surroundingAlcoholCam.x - 1);
					}
					else
					{
						if (distance_x < 0)
						{
							point_x = surroundingAlcoholCam.x - 1;
							point_y = surroundingAlcoholCam.y + surroundingAlcoholCam.h;
						}
						else
						{
							point_x = surroundingAlcoholCam.x + surroundingAlcoholCam.w;
							point_y = surroundingAlcoholCam.y - 1;
						}
					}
				}
				
				percent_distance = (double) ((distance_x * distance_x) + (distance_y * distance_y)) / ((closest_x-point_x) * (closest_x-point_x) + (closest_y-point_y) * (closest_y-point_y));
				
			}
			else
			{
				if (std::abs(distance_x) != 10000)
				{
					percent_distance =  (double) abs(distance_x) / 251;
				}
				else if (std::abs(distance_y) != 10000)
				{
					percent_distance =  (double) abs(distance_y) / 151;
				}
			}
			
			minVelocity = (int) ((291 * percent_distance) + 10);
			
			std::cout << "\nOur minVelocity: " << minVelocity << "\nOur distance percentage: " << percent_distance;
			
			ply->ChangeMaxVelocity((double) (rand() % (291-minVelocity+10)) + minVelocity);
		}
	}
	else
	{
		ply->ChangeMaxVelocity(300);
	}
	
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
	alcoholCam.y = y;
	surroundingAlcoholCam.y = alcoholCam.y - 150;
}
