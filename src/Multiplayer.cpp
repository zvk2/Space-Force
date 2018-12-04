//multiplayer actions that I am currently trying to figure out

//flip player 2
//fire in another direction
//collisions between players and attack/player 
//score board?

#include "Multiplayer.h"

//Public methods
Multiplayer::Multiplayer()

	{
		exists = false;
		frame = 0;
		gRenderer = ply->getRend();
		openGL = gRenderer;

		
	}

void Multiplayer::FlipPlayer2()
{
	if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_NONE;
		else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			flip = SDL_FLIP_HORIZONTAL;
}

void Multiplayer::DamagePlayer1()
{

}

void Multiplayer::DamagePlayer2();
{

}

void Mulitplayer::AttackCollision1()
{

}

void Mulitplayer::AttackCollision2()
{

}