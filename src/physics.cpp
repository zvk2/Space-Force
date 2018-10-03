#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

class Physics
{
	int BACK_LIMIT;
	int FOR_LIMIT;
	int NEUTRAL_X_FOR;
	int NEUTRAL_X_BACK;
	int NEUTRAL_Y_UP;
	int NEUTRAL_Y_DOWN;
	
	Physics(int* x_vel, int *y_vel, int* x_del, int *y_vel, int max_speed, int neutral)
	{
		BACK_LIMIT = -max_speed;
		FOR_LIMIT = max_speed;
		NEUTRAL_X_FOR = neutral;
		NEUTRAL_X_BACK = -neutral;
		NEUTRAL_Y_UP = 0;
		NEUTRAL_Y_DOWN = 0;
	}
	void Speed_Limit(int* x_vel, int *y_vel)
	{
		// Check speed limits

		if (*x_vel < BACK_LIMIT)
		{

			*x_vel = BACK_LIMIT;
		}
		else if (*x_vel > FOR_LIMIT)
		{

			*x_vel = FOR_LIMIT;
		}

		if (*y_vel < BACK_LIMIT)
		{

			*y_vel = BACK_LIMIT;
		}

		else if (*y_vel > FOR_LIMIT)
		{

			*y_vel = FOR_LIMIT;
		}
	}
	//the max speed the player can go

	//What to do when no buttons are pressed

	void Neutral(int *x_del, int *y_del, int *x_vel, int *y_vel)
	{
		if(*x_del == 0)
		{
			if(*x_vel > NEUTRAL_X_FOR)
				{
					*x_del = -1;
				}
			else if (*x_vel < NEUTRAL_X_BACK)
				{
					*x_del = 1;
				}
		}
		if (*y_del == 0)
		{
			if (*y_vel > NEUTRAL_Y_UP)
			{
				*y_del = -1;
			}
			else if (*y_vel < NEUTRAL_Y_DOWN)
			{
				*y_del = 1;
			}
		}

	}
	
}

