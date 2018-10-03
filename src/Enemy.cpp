
#include "INC_SDL.h"

#define MAX_SPEED 50


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

