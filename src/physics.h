#pragma once

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#endif
#include "INC_SDL.h"

class Physics
{

//the max speed the player can go
public:
	Physics(double* x_v, double *y_v, double* x_d, double* y_d,double max_speed, double accel);
	Physics(double x_v, double y_v, double max_speed, double accel);

	void ChangeMove(double ac);
	double GetMove();
	void ChangeMaxSpeed(double speed);
	//What to do when no buttons are pressed
	void Neutral(double timestep);
	void ChangeVelocity(double x_del, double y_del, double timestep);

	//Set the current x velocity
	void setxVelocity(double x);
	//Set the current y velocity
	void setyVelocity(double y);

	//Return the current x velocity
	double getxVelocity();
	//Return the current y velocity
	double getyVelocity();

	private:
	double FOR_LIMIT;
	double BACK_LIMIT;
	double ACCEL;
	double x_vel;
	double y_vel;
	double *x_velp;
	double *y_velp;
	double *x_delp;
	double *y_delp;
	double move;

	// Check speed limits
	void Speed_Limit();
};
