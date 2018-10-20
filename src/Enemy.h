#ifndef ENEMY_H
#define ENEMY_H
#include "INC_SDL.h"
#include "physics.hpp"
#define MAX_SPEED 50

class Enemy
{
	public:

		Enemy(int startingHealth, SDL_Texture* characterImages, int attack);
		
		void LostHealth(int damage);

		void GainedHealth(int heal);

		int GetHealth();

		int GetAttack();

		void IncEnemySpeed(int addedSpeed);

		void DecEnemySpeed(int lostSpeed);

		int GetSpeed();

		//Set the position of the player on screen
		void setPosition(int x, int y);
			
		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep);
		
		// Animate jet propulsion
		void animate(int frames);
		
		//Return the current x velocity
		double getxVel();
		
		//Get the player camera rectangle
		SDL_Rect getEnemyCam();
		
		//Get the current rectangle from the sprite sheet
		SDL_Rect getEnemyRect();
		
		//Get the player sprite sheet
		SDL_Texture* getEnemySheet();



	private:

		/* Member variables:
		 * health, attack
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
	
		double ACCEL = 3600.0;
		double SPEED_LIMIT = 300.0;
		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		int hitPoints;
		int speed;
		double attackPower;

		//Not perm obviously but here as a reminder to store player texture here
		SDL_Texture* enemySheet;
		Physics phys;

		SDL_Rect enemyCam;
		SDL_Rect enemyRect;
		
		double xCoord;
		double yCoord;
	
		void DecrementHealth(int decAmount);

		void IncrementHealth(int incAmount);

		void CheckBoundaries();
		
		void IncrementSpeed(int addedSpeed);

		void DecrementSpeed(int lostSpeed);

};