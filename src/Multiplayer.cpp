//multiplayer actions that I am currently trying to figure out
//flip player 2
//fire in another direction
//collisions between players and attack/player 
//score board?

#include "Multiplayer.h"
#include "PLayer.h"
#include "attack.h"

//Public methods
Multiplayer::Multiplayer(Player* p1, Player* p2, attack* atk1, attack* atk2):ply1(p1), ply2(p2), plyBlast1(atk1), plyBlast2(atk2)
{
	//Get a pointer to both of the players/attacks camBox
	playerCam1 = ply1->getPlayerCamLoc();
	playerCam2 = ply2->getPlayerCamLoc();
	atkCam1 = atk1->getPlayerCamLoc();
	atkCam2 = atk2->getPlayerCamLoc();
}

void Multiplayer::PlayerCollision()
{
	if (SDL_HasIntersection(ply1, ply2))
	{
		return true;
	}

	return false;	
}

bool Player::CheckPlayerCollision(Player* player2, double tstep)
{
	SDL_Rect p2Rect = player2->getPlayerCam();

	//using code from enemy collision in player class to create this player collision 
	//trying to figure this out 

	if (PlayerCollision())
	{
		double newPVelocityx = phys.getxVelocity();
		double newPVelocityy = phys.getyVelocity();
		double newP2Velocityx = player2->getxVel();
		double newP2Velocityy = player2->getyVel();

		xCoord -= (newPVelocityx * tstep);
		yCoord -= (newPVelocityy * tstep);

		if (std::abs(newPVelocityx) > std::abs(newP2Velocityx))
		{
			newP2Velocityx = newP2Velocityx + newPVelocityx;
			newPVelocityx = 0;
		}
		else if (std::abs(newPVelocityx) < std::abs(newP2Velocityx))
		{
			newPVelocityx = newPVelocityx + newP2Velocityx;
			newP2Velocityx = 0;
		}
		else if (newPVelocityx == -newP2Velocityx)
		{
			newPVelocityx = 0;
			newP2Velocityx = 0;
		}

		if (std::abs(newPVelocityy) > std::abs(newP2Velocityy))
		{
			newP2Velocityy = newP2Velocityy + newPVelocityy;
			newPVelocityy = 0;
		}
		else if (std::abs(newPVelocityy) < std::abs(newP2Velocityy))
		{
			newPVelocityy = newPVelocityy + newP2Velocityy;
			newP2Velocityy = 0;
		}
		else if (newPVelocityy == -newP2Velocityy)
		{
			newPVelocityy = 0;
			newP2Velocityy = 0;
		}

		phys.setxVelocity(newPVelocityx);
		phys.setyVelocity(newPVelocityy);
		player2->setVelocity(newP2Velocityx, newP2Velocityy);

		playerCam.x = (int) xCoord;
		playerCam.y = (int) yCoord;

		render->ChangeCoordinates(playerCam.x, playerCam.y, render->z);

		return true;
	}
	return false;
}

//flip player2 in the main when it is rendered 

int Multiplayer::DamagePlayer1()
{
	
}

int Multiplayer::DamagePlayer2();
{
	
}

bool Mulitplayer::AttackCollision1()
{
	//referring to enemy class
	int hits = atk1->hitIntersect(&playerCam2);
	DecrementHealth(hits*ply2->GetAttack());
}

bool Mulitplayer::AttackCollision2()
{
	int hits = atk2->hitIntersect(&playerCam1);
	DecrementHealth(hits*ply1->GetAttack());
}

void Multiplayer::LostHealthPlayer1(int damage1)
{
	DecrementHealth(damage);
}

void Multiplayer::LostHealthPlayer2(int damage2)
{
	DecrementHealth(damage);
}

void Multiplayer::DecrementHealthPlayer1(int decAmount1)
{
	hitPoints1 -= decAmount1;

	if (hitPoints1 <= 0 && life1)
	{
		life1 = false;
		frame1 = 0;
	}
}

void Multiplayer::DecrementHealthPlayer2(int decAmount2)
{
	hitPoints2 -= decAmount2;

	if (hitPoints2 <= 0 && life2)
	{
		life2 = false;
		frame2 = 0;
	}
}