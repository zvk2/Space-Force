
#include "INC_SDL.h"
#include "Enemy.h"
#include <cmath> 
#define MAX_SPEED 50
 
//Public methods 

Enemy::Enemy(int startingHealth, SDL_Texture* characterImages, int attac, attack* player, char _type): 
	hitPoints(startingHealth), enemySheet(characterImages),
	attackPower(attac), phys(0, 0, 300.0, 3600.0), xCoord(1280/8), yCoord(720/2), plyBlast(player), type(_type)
	{
		enemyRect = {0, 0, 144, 87};
		enemyCam = {1280/2, 720/2, 144, 87};
	}

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

//Set the position of the enemy on screen
void Enemy::setPosition(double x, double y)
{
	xCoord = x;
	yCoord = y;
	
	CheckBoundaries();
	
	enemyCam.x = (int) xCoord;
	enemyCam.y = (int) yCoord;
}

//Sets the current velocity of the enemy
void Enemy::setVelocity(double x, double y)
{
	phys.setxVelocity(x);
	phys.setyVelocity(y);
}

//Methods that can be called from model class
void Enemy::move(double xdvel, double ydvel, double tstep)
{
	phys.ChangeVelocity(xdvel, ydvel, tstep);
	
	xCoord += (phys.getxVelocity() * tstep);
	yCoord += (phys.getyVelocity() * tstep);
	
	CheckBoundaries();
	checkAttacked();
	enemyCam.x = (int) xCoord;
	enemyCam.y = (int) yCoord;
}
void Enemy::checkAttacked()
{
	//how many times an enemy been hit
	int hits = plyBlast->hitIntersect(&enemyCam);
}
// Animate jet propulsion
void Enemy::animate(int frames)
{
	enemyRect.x = ((frames / 10) % 4) * enemyRect.w;
}

//Check for collision with the player
void Enemy::checkPlayerCollision(Player* p, double tstep)
{
	if (hasCollision(p))
	{
		double newPVelocityx = p->getxVel();
		double newPVelocityy = p->getyVel();
		double newEVelocityx = phys.getxVelocity();
		double newEVelocityy = phys.getyVelocity();
		
		xCoord -= (newEVelocityx * tstep);
		yCoord -= (newEVelocityy * tstep);
		
		if (std::abs(newPVelocityx) > std::abs(newEVelocityx))
		{
			newEVelocityx = newEVelocityx + newPVelocityx;
			newPVelocityx = 0;
		}
		else if (std::abs(newPVelocityx) < std::abs(newEVelocityx))
		{
			newPVelocityx = newPVelocityx + newEVelocityx;
			newEVelocityx = 0;
		}
		else if (newPVelocityx == -newEVelocityx)
		{
			newPVelocityx = 0;
			newEVelocityx = 0;
		}
		
		if (std::abs(newPVelocityy) > std::abs(newEVelocityy))
		{
			newEVelocityy = newEVelocityy + newPVelocityy;
			newPVelocityy = 0;
		}
		else if (std::abs(newPVelocityy) < std::abs(newEVelocityy))
		{
			newPVelocityy = newPVelocityy + newEVelocityy;
			newEVelocityy = 0;
		}
		else if (newPVelocityy == -newEVelocityy)
		{
			newPVelocityy = 0;
			newEVelocityy = 0;
		}
		
		phys.setxVelocity(newEVelocityx);
		phys.setyVelocity(newEVelocityy);
		p->setVelocity(newPVelocityx, newPVelocityy);
		
		enemyCam.x = (int) xCoord;
		enemyCam.y = (int) yCoord;
	}
}

//Return the current x velocity
double Enemy::getxVel()
{
	return phys.getxVelocity();
}

//Return the current y velocity
double Enemy::getyVel()
{
	return phys.getyVelocity();
}

//Get the enemy camera rectangle
SDL_Rect Enemy::getEnemyCam()
{
	enemyCam.x = (int) xCoord;
	enemyCam.y = (int) yCoord;
	return enemyCam;
}

//Get the current rectangle from the sprite sheet
SDL_Rect Enemy::getEnemyRect()
{
	return enemyRect;
}

//Get the enemy sprite sheet
SDL_Texture* Enemy::getEnemySheet()
{
	return enemySheet;
}

char Enemy::getType()
{
	return type;
}

//Private methods

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
	if (xCoord + enemyCam.w > SCREEN_WIDTH)
		xCoord = SCREEN_WIDTH - enemyCam.w;
	if (yCoord < 0)
		yCoord = 0;
	if (yCoord + enemyCam.h > SCREEN_HEIGHT)
		yCoord = SCREEN_HEIGHT - enemyCam.h;
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

bool Enemy::hasCollision(Player* p)
{
	SDL_Rect pRect = p->getPlayerCam();
	
	//f for faxanaduitis
	if (type == 'f')
	{
		SDL_Rect result;
		
		if (SDL_IntersectRect(&pRect, &enemyCam, &result))
		{
			//Use algebra to calculate slopes and compare them to determine if there is collision
			if ((result.x + result.w - 1) < (enemyCam.x + 33))
			{
				if ((result.y + result.h - 1) < (enemyCam.y + enemyCam.h / 2))
				{
					double enemySlope = (double) ((enemyCam.y + enemyCam.h / 2) - enemyCam.y) / ((enemyCam.x - 1) - (enemyCam.x + 33));
					double playerSlope = (double) ((enemyCam.y + enemyCam.h / 2) - (result.y + result.h - 1)) / ((enemyCam.x - 1) - (result.x + result.w - 1));
					
					if (playerSlope >= enemySlope)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if ((result.y) > (enemyCam.y + enemyCam.h / 2))
				{
					double enemySlope = (double) ((enemyCam.y + enemyCam.h / 2) - (enemyCam.y + enemyCam.h - 1)) / ((enemyCam.x - 1) - (enemyCam.x + 33));
					double playerSlope = (double) ((enemyCam.y + enemyCam.h / 2) - (result.y)) / ((enemyCam.x - 1) - (result.x + result.w - 1));
					
					if (playerSlope <= enemySlope)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else if (result.x >= (enemyCam.x + enemyCam.w - 21))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return SDL_HasIntersection(&pRect, &enemyCam);
	}
}
