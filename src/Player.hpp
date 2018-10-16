
#include "INC_SDL.h"

#define MAX_SPEED 50


class Player
{
	public:
		
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, SDL_Texture* characterImages, int attack): 
			hitPoints(startingHealth), playerSheet(characterImages),
			attackPower(attack), attackModifier(1), defenseModifier(1),
			phys(Physics(0, 0, 300.0, 3600.0)), xCoord(1280/8), yCoord(720/2)
			{
				playerRect = {0, 0, 300, 51};
				playerCam = {1280/2, 720/2, 300, 51};
			}

		//Set the position of the player on screen
		void setPosition(int x, int y)
		{
			playerCam.x = x;
			playerCam.y = y;
		}
			
		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep)
		{
			phys.ChangeVelocity(xdvel, ydvel, tstep);
			
			xCoord += (phys.getxVelocity() * tstep);
			yCoord += (phys.getyVelocity() * tstep);
			
			CheckBoundaries();
			
			playerCam.x = (int) xCoord;
			playerCam.y = (int) yCoord;
		}
		
		// Animate jet propulsion
		void animate(int frames)
		{
			playerRect.x = frames * 300;
		}
		
		//Return the current x velocity
		double getxVel()
		{
			return phys.getxVelocity();
		}
		
		//Get the player camera rectangle
		SDL_Rect getPlayerCam()
		{
			return playerCam;
		}
		
		//Get the current rectangle from the sprite sheet
		SDL_Rect getPlayerRect()
		{
			return playerRect;
		}
		
		//Get the player sprite sheet
		SDL_Texture* getPlayerSheet()
		{
			return playerSheet;
		}
		
		//Subract hit points from the player
		void LostHealth(int damage)
		{
			DecrementHealth(damage/defenseModifier);
		}

		//Add hit points to the player
		void GainedHealth(int heal)
		{
			IncrementHealth(heal);
		}

		//send in double modifiers to increase attack or defense by percentages
		void GainedPowerup(double extraAttack, double extraDefense)
		{
			attackModifier = 1 + extraAttack;
			defenseModifier = 1 + extraDefense;
		}

		//Reset the attack and defence modifiers to normal
		void PowerupEnd()
		{
			attackModifier = 1;
			defenseModifier = 1;
		}

		//Return the player's current health points
		int GetHealth()
		{
			return hitPoints;
		}

		//Return the player's current attack
		int GetAttack()
		{
			return (attackPower*attackModifier);
		}	

	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
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
		double attackModifier;
		double defenseModifier;
		//Not perm obviously but here as a reminder to store player texture here
		SDL_Texture* playerSheet;
		Physics phys;

		SDL_Rect playerCam;
		SDL_Rect playerRect;
		
		double xCoord;
		double yCoord;
		
		void CheckBoundaries()
		{
			// Boundary checks against the window
			if (xCoord < 0)
				xCoord = 0;
			if (xCoord + 300 > SCREEN_WIDTH)
				xCoord = SCREEN_WIDTH - 300;
			if (yCoord < 0)
				yCoord = 0;
			if (yCoord + 51 > SCREEN_HEIGHT)
				yCoord = SCREEN_HEIGHT - 51;
		}
		
		//Private method to decrease player health
		void DecrementHealth(int decAmount)
		{
			hitPoints -= decAmount;
		}

		//Private method to increase player health
		void IncrementHealth(int incAmount)
		{
			hitPoints += incAmount;
		}
};

