#pragma once

#ifndef PLAYER_H
#define PLAYER_H
#include "INC_SDL.h"
#include "physics.h"
#include "attack.h"
#include "music.h"
#include <cmath>
#include "OpenGLRenderer.hpp"
#include <iostream>
#include <cstdlib>
#include <climits>
#define MAX_SPEED 50

class Player
{
	public:
		attack hit;
		//Constructor: takes health, character sheet, and attack value and sets all member vars
		Player(int startingHealth, char* characterImages, int attack, OpenGLRenderer* gRend, bool renderImmediately);

		//attack* attackHit();
		//Set the position of the player on screen
		void setPosition(double x, double y);
		void hasShield(bool has);
		void setAttackColSound(music* sound);

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

		void setAttack(SDL_Rect* attac);

		//Get the player camera rectangle
		SDL_Rect getPlayerCam();

		//Get the current rectangle from the sprite sheet
		SDL_Rect getPlayerRect();


		//Get the player sprite sheet
		char* getPlayerSheet();


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
		OpenGLRenderer* getRend();

		//Get a pointer to the player cam
		SDL_Rect* getPlayerCamLoc();
		double GetMove();
		void ChangeMove(double Accel);
		void ChangeMaxVelocity(double Speed);

		//intracts with health bar
		void HealthBar(RenderObject* health);
		void damage(int hits);
		void shieldLocation(SDL_Rect* protect, int* strength);
		bool* shieldStatus();
		SDL_Rect* shieldInteractions();
		void HitShield(int hits);
		void checkInvincibility(Uint32 frames);

		// Lazy, I know
		RenderObject* render;

	private:

		/* Member variables:
		 * health, attack, and modifiers for powerups
		 * currently a character sheet but can be updated to
		 * OpenGL later
		 */
		SDL_Rect* shield;
		bool protection;
		bool invincible;
		RenderObject* healthBar;
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
		char* playerSheet;
		Physics phys;
		OpenGLRenderer* openGL;
		int *shieldPoint;
		Uint32 iframe;

		SDL_Rect playerCam;
		SDL_Rect playerRect;

		double xCoord;
		double yCoord;

		void CheckBoundaries();

		//Private method to decrease player health
		void DecrementHealth(int decAmount);

		//Private method to increase player health
		void IncrementHealth(int incAmount);

		bool hasCollision(Enemy* e);
};

#include "Enemy.h"

#endif
