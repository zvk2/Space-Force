
#include "Player.h"
#define MAX_SPEED 50



		
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player::Player(int startingHealth, SDL_Texture* characterImages, int attack, SDL_Renderer* gRend): 
			hitPoints(startingHealth), playerSheet(characterImages),
			attackPower(attack), attackModifier(1), defenseModifier(1),
			phys(0, 0, 300.0, 3600.0), xCoord(1280/8), yCoord(720/2), hit(gRend)
			{
				playerRect = {0, 0, 300, 51};
				playerCam = {1280/2, 720/2, 300, 51};	
				gRenderer = gRend;
			}
		void Player::setAttack(SDL_Texture* gAttack, SDL_Rect* attackRect)
		{
			hit.setAttack(gAttack,attackRect);
		}
		//Set the position of the player on screen
		void Player::setPosition(int x, int y)
		{
			playerCam.x = x;
			playerCam.y = y;
		}
		SDL_Renderer* Player::getRend()
		{
			return gRenderer;
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
		}
		
		// Animate jet propulsion
		void Player::animate(int frames)
		{
			playerRect.x = frames * 300;
		}
		
		//Return the current x velocity
		double Player::getxVel()
		{
			return phys.getxVelocity();
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
		SDL_Texture* Player::getPlayerSheet()
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
			if (xCoord + 300 > SCREEN_WIDTH)
				xCoord = SCREEN_WIDTH - 300;
			if (yCoord < 0)
				yCoord = 0;
			if (yCoord + 51 > SCREEN_HEIGHT)
				yCoord = SCREEN_HEIGHT - 51;
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




