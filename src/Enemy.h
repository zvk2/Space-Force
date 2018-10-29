
#ifndef ENEMY_H
#define ENEMY_H
#endif
#include "Player.h"
#include "INC_SDL.h"
#include "physics.h"
#define MAX_SPEED 50

class Enemy
{
	public:

		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Enemy(int startingHealth, SDL_Texture* characterImages, int attack);
		
		//Subract hit points from the player
		void LostHealth(int damage);
		
		//Add hit points to the player
		void GainedHealth(int heal);

		//Return the enemy's current health points
		int GetHealth();
		
		//Return the enemy's current attack
		int GetAttack();

		void IncEnemySpeed(int addedSpeed);

		void DecEnemySpeed(int lostSpeed);

		int GetSpeed();

		//Set the position of the enemy on screen
		void setPosition(double x, double y);
		
		//Sets the current velocity of the enemy
		void setVelocity(double x, double y);	
			
		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep);
		
		// Animate jet propulsion
		void animate(int frames);
		
		//Check for collision with the player
		void checkPlayerCollision(Player* p, double tstep);
		
		//Return the current x velocity
		double getxVel();
		
		//Return the current x velocity
		double getyVel();
		
		//Get the enemy camera rectangle
		SDL_Rect getEnemyCam();
		
		//Get the current rectangle from the sprite sheet
		SDL_Rect getEnemyRect();
		
		//Get the enemy sprite sheet
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

		//Not perm obviously but here as a reminder to store enemy texture here
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