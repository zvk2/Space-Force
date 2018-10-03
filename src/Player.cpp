<<<<<<< HEAD
// checking for apple, else everyone else
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif
=======

#include "INC_SDL.h"

#define MAX_SPEED 50
>>>>>>> 5f6c5c0b223e2f6099745ad2a5772f6b13d47e8f

class Player
{
	public:
		
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, SDL_Texture* characterImages, int attack): 
			hitPoints(startingHealth), playerSheet(characterImages), 
			attackPower(attack),	attackModifier(1), defenseModifier(1)
			{}

		//Methods that can be called from model class

		void LostHealth(int damage)
		{
			DecrementHealth(damage/defenseModifier);
		}

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

		void PowerupEnd()
		{
			attackModifier = 1;
			defenseModifier = 1;
		}

		int GetHealth()
		{
			return hitPoints;
		}

		int GetAttack()
		{
			return (attackPower*attackModifier);
		}

		void IncPlayerSpeed(int addedSpeed)
		{
			IncrementSpeed(addedSpeed);
		}

		void DecPlayerSpeed(int lostSpeed)
		{
			DecrementSpeed(lostSpeed);
		}

		int GetSpeed()
		{
			return speed;
		}
		

	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		int hitPoints;
		int speed;
		double attackPower;
		double attackModifier;
		double defenseModifier;
		//Not perm obviously but here as a reminder to store player texture here
		const SDL_Texture* playerSheet;

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

<<<<<<< HEAD
};
=======
		void IncrementSpeed(int addedSpeed)
		{
			if(speed != MAX_SPEED)
			{
				speed += addedSpeed;
			}
		}

		void DecrementSpeed(int lostSpeed)
		{
			if(speed != -MAX_SPEED)
			{
				speed -= lostSpeed;
			}
		}

};
>>>>>>> 5f6c5c0b223e2f6099745ad2a5772f6b13d47e8f
