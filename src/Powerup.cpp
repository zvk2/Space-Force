
#include "INC_SDL.h"

enum powerType {
	shield,
	lasers,
	bombs,
	invincible
};

class Powerup
{
	public:

		Powerup(double aMod, double dMod, powerType powerup): 
			attackModifier(aMod), defenseModifier(dMod), thisPowerup(powerup)
			{}

		double GetAttackModifier()
		{
			return attackModifier;
		}

		double GetDefenseModifier()
		{
			return defenseModifier;
		}

		powerType GetPowerType()
		{
			return thisPowerup;
		}


	private:

		double attackModifier;
		double defenseModifier;
		powerType thisPowerup;
		//Not perm obviously but here as a reminder to store player texture here
		const SDL_Texture* powerupSheet;


};
