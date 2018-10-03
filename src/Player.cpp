
#include <SDL.h>
#include <SDL_image.h>

class Player
{
	public:
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

		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, SDL_Texture* characterImages, int attack): 
			hitPoints(startingHealth), playerImages(characterImages), 
			attackPower(attack),	attackModifier(1), defenseModifier(1)
			{}

	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		int hitPoints;
		double attackPower;
		double attackModifier;
		double defenseModifier;
		const SDL_Texture* playerImages;

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