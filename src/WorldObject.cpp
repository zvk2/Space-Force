
#include "INC_SDL.h"

#define MAX_SPEED 50

enum objectType {
	asteroid,
	satellite,
	blackHole,
};

class WorldObject
{

	public:

		WorldObject(int objectHealth, int objectGravity, int objectSpeed): 
			strength(objectHealth), gravity(objectGravity), speed(objectSpeed)
			{}

		int GetHealth()
		{
			return strength;
		}

		void LostHealth(int damage)
		{
			DecrementStrength(damage);
		}

		void IncObjectSpeed(int addedSpeed)
		{
			IncrementSpeed(addedSpeed);
		}

		void DecObjectSpeed(int lostSpeed)
		{
			DecrementSpeed(lostSpeed);
		}

		int GetSpeed()
		{
			return speed;
		}


	private:
		
		int strength;
		int gravity;
		int speed;
		//Not perm obviously but here as a reminder to store player texture here
		const SDL_Texture* objectSheet;

		void DecrementStrength(int decAmount)
		{
			strength -= decAmount;
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
