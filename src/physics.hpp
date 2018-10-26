#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "INC_SDL.h"

class Physics
{

//the max speed the player can go
public:
	Physics(double* x_v, double *y_v, double* x_d, double* y_d,double max_speed, double accel);
	Physics(double x_v, double y_v, double max_speed, double accel);
	
	
//What to do when no buttons are pressed
	void Neutral(double timestep);
	void ChangeVelocity(double x_del, double y_del, double timestep);
	
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
	
	// Check speed limits
	void Speed_Limit();
};
#endif
