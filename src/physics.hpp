#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

class Physics
{

//the max speed the player can go
public:
	Physics(int* x_v, int *y_v, int* x_d, int* y_d,int max_speed, int neutral_pos, int neutral_neg):
	BACK_LIMIT{-max_speed}, FOR_LIMIT{max_speed},NEUTRAL_X_FOR{neutral_pos},NEUTRAL_X_BACK{neutral_neg},
NEUTRAL_Y_DOWN{0}, NEUTRAL_Y_UP{0},x_vel{x_v},y_vel{y_v},x_del{x_d}, y_del{y_d}
	{
	}
	void Speed_Limit()
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
//What to do when no buttons are pressed
	void Neutral()
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
	private:
	int FOR_LIMIT;
	int BACK_LIMIT;
	int NEUTRAL_X_FOR;
	int NEUTRAL_X_BACK;
	int NEUTRAL_Y_UP;
	int NEUTRAL_Y_DOWN;
	int *x_vel;
	int *y_vel;
	int *x_del;
	int *y_del;
};
