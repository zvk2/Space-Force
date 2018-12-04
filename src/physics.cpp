#include "physics.h"

//the max speed the player can go

	Physics::Physics(double* x_v, double *y_v, double* x_d, double* y_d,double max_speed, double accel):
	BACK_LIMIT{-max_speed}, FOR_LIMIT{max_speed},ACCEL{accel}, x_velp{x_v},y_velp{y_v},x_delp{x_d}, y_delp{y_d}
	{
		move = ACCEL;
	}
	Physics::Physics(double x_v, double y_v, double max_speed, double accel):
	BACK_LIMIT{-max_speed}, FOR_LIMIT{max_speed},ACCEL{accel}, x_vel{x_v},y_vel{y_v}
	{
		move = ACCEL;
	}


//What to do when no buttons are pressed
	void Physics::Neutral(double timestep)
	{
		if (*x_delp == 0)
		{
			if (*x_velp > 0)
			{
				if (*x_velp < (ACCEL * timestep))
					*x_velp = 0;
				else
					*x_velp -= (ACCEL * timestep);
			}
			else if (*x_velp < 0)
			{
				if (-(*x_velp) < (ACCEL * timestep))
					*x_velp = 0;
				else
					*x_velp += (ACCEL * timestep);
			}
		}
		else
			*x_velp += *x_delp;

		if (*y_delp == 0)
		{
			if (*y_velp > 0)
			{
				if (*y_velp < (ACCEL * timestep))
					*y_velp = 0;
				else
					*y_velp -= (ACCEL * timestep);
			}
			else if (*y_velp < 0)
			{
				if (-(*y_velp) < (ACCEL * timestep))
					*y_velp = 0;
				else
					*y_velp += (ACCEL * timestep);
			}
		}
		else
			*y_velp += *y_delp;

	}
	void Physics::ChangeVelocity(double x_del, double y_del, double timestep)
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
		{
			x_vel += x_del;
		}

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

	//move is the accelerate in the main.cpp
	void Physics::ChangeMove(double ac)
	{
		move = ac;
	}

	double Physics::GetMove()
	{
		return move;
	}

	void Physics::ChangeMaxSpeed(double speed)
	{
		BACK_LIMIT = -speed;
		FOR_LIMIT = speed;
	}

	//Set the current x velocity
	void Physics::setxVelocity(double x)
	{
		x_vel = x;
	}

	//Set the current y velocity
	void Physics::setyVelocity(double y)
	{
		y_vel = y;
	}

	//Return the current x velocity
	double Physics::getxVelocity()
	{
		return x_vel;
	}

	//Return the current y velocity
	double Physics::getyVelocity()
	{
		return y_vel;
	}
	void Physics::Speed_Limit()
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
