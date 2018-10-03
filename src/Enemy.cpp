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

class Enemy
{
	public:

		Enemy(int startingHealth, SDL_Texture* characterImages, int attack): 
			hitPoints(startingHealth), enemySheet(characterImages), attackPower(attack)
			{}

		void LostHealth(int damage)
		{
			DecrementHealth(damage);
		}

		void GainedHealth(int heal)
		{
			IncrementHealth(heal);
		}

		int GetHealth()
		{
			return hitPoints;
		}

		int GetAttack()
		{
			return attackPower;
		}

		void IncEnemySpeed(int addedSpeed)
		{
			IncrementSpeed(addedSpeed);
		}

		void DecEnemySpeed(int lostSpeed)
		{
			DecrementSpeed(lostSpeed);
		}

		int GetSpeed()
		{
			return speed;
		}


	private:

		/* Member variables:
		 * health, attack
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		int hitPoints;
		int attackPower;
		int speed;
		//Not perm obviously but here as a reminder to store player texture here
		const SDL_Texture* enemySheet;

		void DecrementHealth(int decAmount)
		{
			hitPoints -= decAmount;
		}

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
