#pragma once

#ifndef PLAYER_H
#define PLAYER_H
#include "INC_SDL.h"
#include "Enemy.h"
#include "physics.h"
#include "attack.h"
#include <cmath>
#define MAX_SPEED 50

class Player
{
	public:
		attack hit;
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, SDL_Texture* characterImages, int attack, SDL_Renderer* gRend);

		//attack* attackHit();
		//Set the position of the player on screen
		void setPosition(double x, double y);
	

        //Int version
        void setPosition(int x, int y);

		//Sets the current velocity of the player
		void setVelocity(double x, double y);

		//Methods that can be called from model class
		void move(double xdvel, double ydvel, double tstep);


		// Animate jet propulsion
		void animate(int frames);

		//Check for collision with an enemy
		bool checkEnemyCollision(class Enemy* e, double tstep);

		//Return the current x velocity
		double getxVel();

		//Return the current y velocity
		double getyVel();

		void setAttack(SDL_Texture* gAtt, SDL_Rect* attac);

		//Get the player camera rectangle
		SDL_Rect getPlayerCam();

		//Get the current rectangle from the sprite sheet
		SDL_Rect getPlayerRect();


		//Get the player sprite sheet
		SDL_Texture* getPlayerSheet();


		//Subract hit points from the player
		void LostHealth(int damage);


		//Add hit points to the player
		void GainedHealth(int heal);

		//send in double modifiers to increase attack or defense by percentages
		void GainedPowerup(double extraAttack, double extraDefense);


		//Reset the attack and defence modifiers to normal
		void PowerupEnd();


		//Return the player's current health points
		int GetHealth();


		//Return the player's current attack
		int GetAttack();
		SDL_Renderer* getRend();
		SDL_Rect* getPlayerCamLoc();
		double GetMove();
		void ChangeMove(double Accel);
		
		//intracts with health bar
		void HealthBar(SDL_Rect* health);
		void damage(int hits);
	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		SDL_Rect* healthBar;
		double ACCEL;
		double SPEED_LIMIT;
		const int SCREEN_WIDTH = 1280;
		const int SCREEN_HEIGHT = 720;
		int hitPoints;
		int speed;
		double attackPower;
		double attackModifier;
		double defenseModifier;
		//Not perm obviously but here as a reminder to store player texture here
		SDL_Texture* playerSheet;
		Physics phys;
		SDL_Renderer* gRenderer;

		SDL_Rect playerCam;
		SDL_Rect playerRect;

		double xCoord;
		double yCoord;

		void CheckBoundaries();

		//Private method to decrease player health
		void DecrementHealth(int decAmount);

		//Private method to increase player health
		void IncrementHealth(int incAmount);
};

#endif
