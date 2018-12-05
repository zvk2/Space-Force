//multiplayer actions that I am currently trying to figure out
//flip player 2
//fire in another direction
//collisions between players and attack/player 
//score board?

#include "Multiplayer.h"
#include "Player.h"
#include "attack.h"

int hitPoints1 = 20;
int hitPoints2 = 20;

//Public methods
Multiplayer::Multiplayer(Player* p1, Player* p2, attack* atk1, attack* atk2):ply1(p1), ply2(p2), plyBlast1(atk1), plyBlast2(atk2)
{
	//Get a pointer to both of the players/attacks camBox
	playerCam1 = ply1->getPlayerCamLoc();
	playerCam2 = ply2->getPlayerCamLoc();
}

void Multiplayer::PlayerCollision()
{
	if (SDL_HasIntersection(playerCam1, playerCam2))
	{
		return true;
	}

	return false;	
}

bool Multiplayer::CheckPlayerCollision(Player* player2, double tstep)
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

void Multiplayer::TotalDamagePlayer1()
{
	if(hitPoints1 > 0)
	{
		//player 1 still alive
	}
	else
	{
		//player2 dead
	}
}

void Multiplayer::TotalDamagePlayer2();
{
	if(hitPoints2 > 0)
	{
		//player2 still alive
	}
	else
	{
		//player2 dead
	}
}

void Mulitplayer::AttackCollision1()
{
	//referring to enemy class
	int hits1 = plyBlast1->hitIntersect(&playerCam2);
	DecrementHealthPlayer2(hits1*ply2->GetAttack());
}

void Mulitplayer::AttackCollision2()
{
	int hits2 = plyBlast2->hitIntersect(&playerCam1);
	DecrementHealthPlayer1(hits2*ply1->GetAttack());
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