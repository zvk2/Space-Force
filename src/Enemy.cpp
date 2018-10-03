
#include <SDL.h>
#include <SDL_image.h>

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


	private:

		/* Member variables:
		 * health, attack
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		int hitPoints;
		int attackPower;
		const SDL_Texture* enemySheet;

		void DecrementHealth(int decAmount)
		{
			hitPoints -= decAmount;
		}

		void IncrementHealth(int incAmount)
		{
			hitPoints += incAmount;
		}

};