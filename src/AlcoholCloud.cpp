#include "AlcoholCloud.h"

AlcoholCloud::AlcoholCloud(Player* p, Enemy* e, attack* atk):ply(p), emy(e), plyBlast(atk)
{
	//~ std::string initTexture = "resources/imgs/Alcohol_Cloud.png";
	//~ cloudTexture = initTexture.c_str();

	//~ initTexture = "resources/imgs/Alcohol_Cloud_Flare_Up.png";
	//~ flareUpTexture = initTexture.c_str();

	openGL = ply->getRend();
	spriteBox = {0, 0, 500, 300};

	//Get a pointer to the player's camBox
	playerCam = ply->getPlayerCamLoc();
	enemyCam = emy->getEnemyCamLoc();
	alcoholCam = {1530, 150, 500, 300};
	surroundingAlcoholCam = {1280, 0, 1000, 600};
	onScreen = false;
	flareUp = false;
	flareTime = 0;
	frame = 0;
	delay = 0;

	// Only one render for item and one for protect
	cloudRender = new RenderObject(
		alcoholCam.x, alcoholCam.y, 0.5, openGL->allBufferAttributes["resources/imgs/Alcohol_Cloud.png"]
	);
	openGL->AppendRenderObject(cloudRender);

	//~ std::cout << "Cloud Spawn" << std::endl;
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

	cloudRender->IterateFrame();
	//~ flareUpRender->IterateFrame();

	if (plyBlast->hitIntersect(&alcoholCam) > 0)
	{
		flareUp = true;
	}

	//Places object to screen
	if (flareUp)
	{
		//Increase the amount of flare up time by one
		flareTime += 1;

		if (flareTime == 1)
		{
			// Change cloudRender to use the flare up attributes
			cloudRender->bufferAttributes = openGL->allBufferAttributes["resources/imgs/Alcohol_Cloud_Flare_Up.png"];
			cloudRender->currentBufferID = cloudRender->bufferAttributes.bufferIDStart;
		}

		cloudRender->ChangeCoordinates(
			alcoholCam.x,
			alcoholCam.y,
			cloudRender->z
		);

		//~ SDL_RenderCopy(gRenderer, spriteFlare, &spriteBox, &alcoholCam);
		// WHY DID I DO SOMETHING SO DUMB?
		//~ flareUpRender->ChangeCoordinates(
			//~ alcoholCam.x,
			//~ alcoholCam.y,
			//~ flareUpRender->z
		//~ );
	}
	else
	{
		//~ SDL_RenderCopy(gRenderer, sprite, &spriteBox, &alcoholCam);
		cloudRender->ChangeCoordinates(
			alcoholCam.x,
			alcoholCam.y,
			cloudRender->z
		);
	}

	checkPlayerCollision();
	checkEnemyCollision();

	//Move the alcohol cloud the left by one pixel
	alcoholCam.x = alcoholCam.x - 1;
	surroundingAlcoholCam.x = surroundingAlcoholCam.x - 1;

	//Used to determine how long to call the alcohol cloud for
	if ((surroundingAlcoholCam.x + surroundingAlcoholCam.w > 0) && (flareTime < 250))
	{
		onScreen = true;
	}
	else
	{
		onScreen = false;
		flareUp = false;
		surroundingAlcoholCam.x = 1280;
		surroundingAlcoholCam.y = 0;
		alcoholCam.x = 1530;
		alcoholCam.y = 150;
		delay = 0;
		flareTime = 0;

		// Make sure it is using the normal cloud buffer again
		cloudRender->bufferAttributes = openGL->allBufferAttributes["resources/imgs/Alcohol_Cloud.png"];
		cloudRender->currentBufferID = cloudRender->bufferAttributes.bufferIDStart;

		cloudRender->ChangeCoordinates(
			alcoholCam.x,
			alcoholCam.y,
			cloudRender->z
		);
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

//Set the y position of the alcohol cloud
void AlcoholCloud::setYPosition(int y)
{
	alcoholCam.y = y;
	surroundingAlcoholCam.y = alcoholCam.y - 150;
}

//Private methods
//Check collision with the player
void AlcoholCloud::checkPlayerCollision()
{
	SDL_Rect result;

	//Slow the player down upon entering the surrounding area of the alcohol cloud
	if (SDL_IntersectRect(&surroundingAlcoholCam, playerCam, &result))
	{
		//Slow the player down if inside of the alcohol cloud
		if (SDL_HasIntersection(&alcoholCam, playerCam))
		{
			if (flareUp)
			{
				ply->LostHealth(1);
				ply->damage(1);
			}

			ply->ChangeMaxVelocity((double) (rand() % 291) + 10);
		}
		//Slow the player down in proportion to the distance from the cloud
		else
		{
			int distance_x, distance_y, closestA_x, closestA_y, closestPly_x, closestPly_y, closestS_x, closestS_y, minVelocity;
			double percent_distance;

			//Find the closest points to the player
			if ((result.x + result.w) <= alcoholCam.x)
			{
				distance_x = (result.x + result.w - 1) - alcoholCam.x;
				closestA_x = alcoholCam.x;
				closestPly_x = result.x + result.w - 1;
				closestS_x = surroundingAlcoholCam.x - 1;
			}
			else if (result.x >= (alcoholCam.x + alcoholCam.w))
			{
				distance_x = result.x - (alcoholCam.x + alcoholCam.w - 1);
				closestA_x = (alcoholCam.x + alcoholCam.w - 1);
				closestPly_x = result.x;
				closestS_x = surroundingAlcoholCam.x + surroundingAlcoholCam.w;
			}
			else
			{
				distance_x = 10000;
			}

			if ((result.y + result.h) <= alcoholCam.y)
			{
				distance_y = (result.y + result.h - 1) - alcoholCam.y;
				closestA_y = alcoholCam.y;
				closestPly_y = result.y + result.h - 1;
				closestS_y = surroundingAlcoholCam.y - 1;
			}
			else if (result.y >= (alcoholCam.y + alcoholCam.h))
			{
				distance_y = result.y - (alcoholCam.y + alcoholCam.h - 1);
				closestA_y = (alcoholCam.y + alcoholCam.h - 1);
				closestPly_y = result.y;
				closestS_y = surroundingAlcoholCam.y + surroundingAlcoholCam.h;
			}
			else
			{
				distance_y = 10000;
			}
			//Calculate the percent distance by using algebra to determine
			//a slope and a point on the border of the surrounding alcohol cam
			if ((distance_x != 10000) && (distance_y != 10000))
			{
				double slope, alcoholSlope;
				int point_x, point_y;

				alcoholSlope = (double) (alcoholCam.y - surroundingAlcoholCam.y - 1) / (alcoholCam.x - surroundingAlcoholCam.x - 1);
				slope = (double) (distance_y) / (distance_x);

				//Positive slope
				if (slope > 0)
				{
					if (slope < alcoholSlope)
					{
						point_x = closestS_x;
						point_y = (slope * ((closestS_x) - closestPly_x)) + closestPly_y;
					}
					else if (slope > alcoholSlope)
					{
						point_x = (((closestS_y) - closestPly_y) + (slope * closestPly_x)) / slope;
						point_y = closestS_y;
					}
					else
					{
						point_x = closestS_x;
						point_y = closestS_y;
					}
				}
				//Negative slope
				else
				{
					if (slope < -alcoholSlope)
					{
						point_x = (((closestS_y) - closestPly_y) + (slope * closestPly_x)) / slope;
						point_y = closestS_y - 1;
					}
					else if (slope > -alcoholSlope)
					{
						point_x = closestS_x;
						point_y = (slope * ((closestS_x) - closestPly_x)) + closestPly_y;
					}
					else
					{
						point_x = closestS_x;
						point_y = closestS_y;
					}
				}

				//Calculate the percent distance by using the Pythagorean theorem
				percent_distance = (double) sqrt((distance_x * distance_x) + (distance_y * distance_y)) / sqrt(((closestA_x-point_x) * (closestA_x-point_x)) + ((closestA_y-point_y) * (closestA_y-point_y)));

				//Ensure the program doesn't crash by not dividing by zero
				if (percent_distance > 1)
				{
					percent_distance = 1;
				}
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

			minVelocity = (int) ((290 * percent_distance) + 10);
			ply->ChangeMaxVelocity((double) (rand() % (301-minVelocity) + minVelocity));
		}
	}
	//If out of the cloud, reset the player's max speed to normal
	else
	{
		ply->ChangeMaxVelocity(300);
	}
}

void AlcoholCloud::checkEnemyCollision()
{
	SDL_Rect result;

	//Slow the player down upon entering the surrounding area of the alcohol cloud
	if (SDL_IntersectRect(&surroundingAlcoholCam, enemyCam, &result))
	{
		//Slow the player down if inside of the alcohol cloud
		if (SDL_HasIntersection(&alcoholCam, enemyCam))
		{
			if (flareUp)
			{
				emy->LostHealth(1);
			}

			emy->ChangeMaxVelocity((double) (rand() % 291) + 10);
		}
		//Slow the player down in proportion to the distance from the cloud
		else
		{
			int distance_x, distance_y, closestA_x, closestA_y, closestEmy_x, closestEmy_y, closestS_x, closestS_y, minVelocity;
			double percent_distance;

			//Find the closest points to the player
			if ((result.x + result.w) <= alcoholCam.x)
			{
				distance_x = (result.x + result.w - 1) - alcoholCam.x;
				closestA_x = alcoholCam.x;
				closestEmy_x = result.x + result.w - 1;
				closestS_x = surroundingAlcoholCam.x - 1;
			}
			else if (result.x >= (alcoholCam.x + alcoholCam.w))
			{
				distance_x = result.x - (alcoholCam.x + alcoholCam.w - 1);
				closestA_x = (alcoholCam.x + alcoholCam.w - 1);
				closestEmy_x = result.x;
				closestS_x = surroundingAlcoholCam.x + surroundingAlcoholCam.w;
			}
			else
			{
				distance_x = 10000;
			}

			if ((result.y + result.h) <= alcoholCam.y)
			{
				distance_y = (result.y + result.h - 1) - alcoholCam.y;
				closestA_y = alcoholCam.y;
				closestEmy_y = result.y + result.h - 1;
				closestS_y = surroundingAlcoholCam.y - 1;
			}
			else if (result.y >= (alcoholCam.y + alcoholCam.h))
			{
				distance_y = result.y - (alcoholCam.y + alcoholCam.h - 1);
				closestA_y = (alcoholCam.y + alcoholCam.h - 1);
				closestEmy_y = result.y;
				closestS_y = surroundingAlcoholCam.y + surroundingAlcoholCam.h;
			}
			else
			{
				distance_y = 10000;
			}
			//Calculate the percent distance by using algebra to determine
			//a slope and a point on the border of the surrounding alcohol cam
			if ((distance_x != 10000) && (distance_y != 10000))
			{
				double slope, alcoholSlope;
				int point_x, point_y;

				alcoholSlope = (double) (alcoholCam.y - surroundingAlcoholCam.y - 1) / (alcoholCam.x - surroundingAlcoholCam.x - 1);
				slope = (double) (distance_y) / (distance_x);

				//Positive slope
				if (slope > 0)
				{
					if (slope < alcoholSlope)
					{
						point_x = closestS_x;
						point_y = (slope * ((closestS_x) - closestEmy_x)) + closestEmy_y;
					}
					else if (slope > alcoholSlope)
					{
						point_x = (((closestS_y) - closestEmy_y) + (slope * closestEmy_x)) / slope;
						point_y = closestS_y;
					}
					else
					{
						point_x = closestS_x;
						point_y = closestS_y;
					}
				}
				//Negative slope
				else
				{
					if (slope < -alcoholSlope)
					{
						point_x = (((closestS_y) - closestEmy_y) + (slope * closestEmy_x)) / slope;
						point_y = closestS_y - 1;
					}
					else if (slope > -alcoholSlope)
					{
						point_x = closestS_x;
						point_y = (slope * ((closestS_x) - closestEmy_x)) + closestEmy_y;
					}
					else
					{
						point_x = closestS_x;
						point_y = closestS_y;
					}
				}

				//Calculate the percent distance by using the Pythagorean theorem
				percent_distance = (double) sqrt((distance_x * distance_x) + (distance_y * distance_y)) / sqrt(((closestA_x-point_x) * (closestA_x-point_x)) + ((closestA_y-point_y) * (closestA_y-point_y)));

				//Ensure the program doesn't crash by not dividing by zero
				if (percent_distance > 1)
				{
					percent_distance = 1;
				}
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

			minVelocity = (int) ((290 * percent_distance) + 10);
			emy->ChangeMaxVelocity((double) (rand() % (301-minVelocity) + minVelocity));
		}
	}
	//If out of the cloud, reset the enemy's max speed to normal
	else
	{
		emy->ChangeMaxVelocity(300);
	}
}
