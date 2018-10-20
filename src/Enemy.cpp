
#include "INC_SDL.h"
#include "Enemy.h"

#define MAX_SPEED 50
   


Enemy(int startingHealth, SDL_Texture* characterImages, int attack): 
	hitPoints(startingHealth), enemySheet(characterImages), attackPower(attack)
	{}

void Enemy::LostHealth(int damage)
{
	DecrementHealth(damage);
}

void Enemy::GainedHealth(int heal)
{
	IncrementHealth(heal);
}

int Enemy::GetHealth()
{
	return hitPoints;
}

int Enemy::GetAttack()
{
	return attackPower;
}

void Enemy::IncEnemySpeed(int addedSpeed)
{
	IncrementSpeed(addedSpeed);
}

void Enemy::DecEnemySpeed(int lostSpeed)
{
	DecrementSpeed(lostSpeed);
}

int Enemy::GetSpeed()
{
	return speed;
}

//Set the position of the player on screen
void Enemy::setPosition(int x, int y)
{
	playerCam.x = x;
	playerCam.y = y;
}
	
//Methods that can be called from model class
void Enemy::move(double xdvel, double ydvel, double tstep)
{
	phys.ChangeVelocity(xdvel, ydvel, tstep);
	
	xCoord += (phys.getxVelocity() * tstep);
	yCoord += (phys.getyVelocity() * tstep);
	
	CheckBoundaries();
	
	playerCam.x = (int) xCoord;
	playerCam.y = (int) yCoord;
}

// Animate jet propulsion
void Enemy::animate(int frames)
{
	playerRect.x = frames * 300;
}

//Return the current x velocity
double Enemy::getxVel()
{
	return phys.getxVelocity();
}

//Get the enemy camera rectangle
SDL_Rect Enemy::getEnemyCam()
{
	return playerCam;
}

//Get the current rectangle from the sprite sheet
SDL_Rect Enemy::getEnemyRect()
{
	return playerRect;
}

//Get the enemy sprite sheet
SDL_Texture* Enemy::getEnemySheet()
{
	return playerSheet;
}

void Enemy::DecrementHealth(int decAmount)
{
	hitPoints -= decAmount;
}

void Enemy::IncrementHealth(int incAmount)
{
	hitPoints += incAmount;
}

void Enemy::CheckBoundaries()
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

void Enemy::IncrementSpeed(int addedSpeed)
{
	if(speed != MAX_SPEED)
	{
		speed += addedSpeed;
	}
}

void Enemy::DecrementSpeed(int lostSpeed)
{
	if(speed != -MAX_SPEED)
	{
		speed -= lostSpeed;
	}
}


