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

	Physics(double x_v, double y_v, double max_speed, double accel):
	BACK_LIMIT{-max_speed}, FOR_LIMIT{max_speed},ACCEL{accel}, x_vel{x_v},y_vel{y_v}

	{
	}
	
	
//What to do when no buttons are pressed
	void ChangeVelocity(double x_del, double y_del, double timestep)
	{
		if (x_del == 0.0) 
		{
			if (x_vel > 0) 
			{
				if (x_vel < (ACCEL * timestep))
					x_vel = 0;
				else
					x_vel -= (ACCEL * timestep);
			}
			else if (x_vel < 0) 
			{
				if (-(x_vel) < (ACCEL * timestep))
					x_vel = 0;
				else
					x_vel += (ACCEL * timestep);
			}
		}
		else
			x_vel += x_del;
		
		if (y_del == 0.0)
		{
			if (y_vel > 0)
			{
				if (y_vel < (ACCEL * timestep))
					y_vel = 0;
				else
					y_vel -= (ACCEL * timestep);
			}
			else if (y_vel < 0)
			{
				if (-(y_vel) < (ACCEL * timestep))
					y_vel = 0;
				else
					y_vel += (ACCEL * timestep);
			}
		}
		else
		{
			y_vel += y_del;
		}
	
		Speed_Limit();
	}
	
	//Return the current x velocity
	double getxVelocity()
	{
		return x_vel;
	}
	
	//Return the current y velocity
	double getyVelocity()
	{
		return y_vel;
	}
	
	private:
	double FOR_LIMIT;
	double BACK_LIMIT;
	double ACCEL;
	double x_vel;
	double y_vel;
	
	// Check speed limits
	void Speed_Limit()
	{
			if (x_vel < BACK_LIMIT)
			{
				x_vel = BACK_LIMIT;
			}
			else if (x_vel > FOR_LIMIT)
			{
				x_vel = FOR_LIMIT;
			}
			if (y_vel < BACK_LIMIT)
			{
				y_vel = BACK_LIMIT;
			}
			else if (y_vel > FOR_LIMIT)
			{
				y_vel = FOR_LIMIT;
			}
	}
};
