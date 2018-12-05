
#include "Player.h"
#define MAX_SPEED 50

//global variable for multiplayer

//Constructor: takes health, character sheet, and attack value and sets all member vars
Player::Player(int startingHealth, char* characterImages, int attack, OpenGLRenderer* gRend, bool renderImmediately):
	hitPoints(startingHealth), playerSheet(characterImages),
	attackPower(attack), attackModifier(1), defenseModifier(1),
	phys(0, 0, 300.0, 3600.0), xCoord(1280/8), yCoord(720/2), hit(gRend)
	{
		openGL = gRend;

		playerRect = {0, 0, 240, 51};
		playerCam = {1280/2, 720/2, 240, 51};
		//~ gRenderer = gRend;
		protection = false;
		invincible = false;
		iframe = 0;

		// May need to rethink z index
		render = new RenderObject(
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, openGL->allBufferAttributes[characterImages]
		);

		if (renderImmediately)
		{
			openGL->AppendRenderObject(render);
		}
	}

//checking collisions between players in multiplayer
// void Player::MultiplayerCollision(){
// 	other -> getPlayerCamLoc();
// }

void Player::setAttack(SDL_Rect* attackRect)
{
	hit.setAttack(attackRect);
}
void Player::setAttackColSound(music* sound)
{
	hit.setColSound(sound);
}
void Player::hasShield(bool has)
{
	protection = has;
}
void Player::shieldLocation(SDL_Rect* protect, int* strength)
{
	shield = protect;
	shieldPoint = strength;
}
bool* Player::shieldStatus()
{
	return &protection;
}
SDL_Rect* Player::shieldInteractions()
{
	return shield;
}
void Player::HitShield(int hits)
{
	*shieldPoint = *shieldPoint - hits;
}

void Player::checkInvincibility(Uint32 frames)
{
	if (frames > iframe)
	{
		invincible = false;
	}
}

//Set the position of the player on screen
void Player::setPosition(int x, int y)
{
	playerCam.x = x;
	playerCam.y = y;

	render->ChangeCoordinates(x, y, render->z);
}
void Player::HealthBar(RenderObject* health)//needed to access healthbar other object classes
{
	healthBar = health;
}
void Player::damage(int hits)//other objects effect on health
{
	if (!invincible)
	{
		for(int i = 0; i <hits; i++)
			healthBar->ForceFrame();
		invincible = true;
		iframe = SDL_GetTicks() + 1500;
	}
}
//attack* Player::attackHit()
//{
	//return *hit;
//	}
OpenGLRenderer* Player::getRend()
{
	return openGL;
}

//Change the current velocity of the player based on the current velocity
void Player::changeVelocity(double xdvel, double ydvel, double tstep)
{
	phys.ChangeVelocity(xdvel, ydvel, tstep);
}

//Methods that can be called from model class
void Player::move(double xdvel, double ydvel, double tstep)
{
	phys.ChangeVelocity(xdvel, ydvel, tstep);

	xCoord += (phys.getxVelocity() * tstep);
	yCoord += (phys.getyVelocity() * tstep);

	CheckBoundaries();

	playerCam.x = (int) xCoord;
	playerCam.y = (int) yCoord;

	render->ChangeCoordinates(xCoord, yCoord, render->z);
}

// Animate jet propulsion
void Player::animate(int frames)
{
	playerRect.x = (frames % 4) * 240;
	render->IterateFrame();
}

//Sets the current velocity of the player
void Player::setVelocity(double x, double y)
{
	phys.setxVelocity(x);
	phys.setyVelocity(y);
}
void Player::setXVelocity(double x)
{
	phys.setxVelocity(x);
}
void Player::setYVelocity(double y)
{
	phys.setyVelocity(y);
}

//Return the current x velocity
double Player::getxVel()
{
	return phys.getxVelocity();
}

double Player::getyVel()
{
	return phys.getyVelocity();
}

//Get the player camera rectangle
SDL_Rect Player::getPlayerCam()
{
	return playerCam;
}

SDL_Rect* Player::getPlayerCamLoc()
{
	return &playerCam;
}

double Player::GetMove()
{
	return phys.GetMove();
}

void Player::ChangeMaxVelocity(double Speed)
{
	phys.ChangeMaxSpeed(Speed);
}

void Player::ChangeMove(double Accel)
{
	phys.ChangeMove(Accel);
}
//Get the current rectangle from the sprite sheet
SDL_Rect Player::getPlayerRect()
{
	return playerRect;
}

//Get the player sprite sheet
char* Player::getPlayerSheet()
{
	return playerSheet;
}

//Subract hit points from the player
void Player::LostHealth(int damage)
{
	DecrementHealth(damage/defenseModifier);
}

//Add hit points to the player
void Player::GainedHealth(int heal)
{
	IncrementHealth(heal);
}

//send in double modifiers to increase attack or defense by percentages
void Player::GainedPowerup(double extraAttack, double extraDefense)
{
	attackModifier = 1 + extraAttack;
	defenseModifier = 1 + extraDefense;
}

//Reset the attack and defence modifiers to normal
void Player::PowerupEnd()
{
	attackModifier = 1;
	defenseModifier = 1;
}

//Return the player's current health points
int Player::GetHealth()
{
	return hitPoints;
}

//Return the player's current attack
int Player::GetAttack()
{
	return (attackPower*attackModifier);
}

void Player::CheckBoundaries()
{
	// Boundary checks against the window
	if (xCoord < 0)
		xCoord = 0;
	if (xCoord + 240 > SCREEN_WIDTH)
		xCoord = SCREEN_WIDTH - 240;
	if (yCoord < 0)
		yCoord = 0;
	if (yCoord + 51 > SCREEN_HEIGHT)
		yCoord = SCREEN_HEIGHT - 51;

	render->ChangeCoordinates(xCoord, yCoord, render->z);
}

//Private method to decrease player health
void Player::DecrementHealth(int decAmount)
{
	hitPoints -= decAmount;
}

//Private method to increase player health
void Player::IncrementHealth(int incAmount)
{
	hitPoints += incAmount;
}

//Check for collision with an enemy
bool Player::checkEnemyCollision(Enemy* e, double tstep)
{
	SDL_Rect eRect = e->getEnemyCam();

	if (hasCollision(e))
	{
		double newPVelocityx = phys.getxVelocity();
		double newPVelocityy = phys.getyVelocity();
		double newEVelocityx = e->getxVel();
		double newEVelocityy = e->getyVel();

		xCoord -= (newPVelocityx * tstep);
		yCoord -= (newPVelocityy * tstep);

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

		phys.setxVelocity(newPVelocityx);
		phys.setyVelocity(newPVelocityy);
		e->setVelocity(newEVelocityx, newEVelocityy);

		playerCam.x = (int) xCoord;
		playerCam.y = (int) yCoord;

		render->ChangeCoordinates(playerCam.x, playerCam.y, render->z);

		return true;
	}
	return false;
}

bool Player::hasCollision(Enemy* e)
{
	SDL_Rect enemyCam = e->getEnemyCam();

	//f for faxanaduitis
	if (e->getType() == 'f')
	{
		SDL_Rect result;

		if (SDL_IntersectRect(&playerCam, &enemyCam, &result))
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
		return SDL_HasIntersection(&playerCam, &enemyCam);
	}
}
