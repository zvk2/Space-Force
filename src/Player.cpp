

#include "Player.h"
#define MAX_SPEED 50
#include <iostream>
#include <cstdlib>
#include <climits>



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
			playerRect.x = (frames % 4) * 300;
		}

		//Sets the current velocity of the player
		void Player::setVelocity(double x, double y)
		{
			phys.setxVelocity(x);
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

	//Check for collision with an enemy
bool Player::checkEnemyCollision(Enemy* e, double tstep)
{
	SDL_Rect eRect = e->getEnemyCam();

	if (SDL_HasIntersection(&eRect, &playerCam))
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
		return true;
	}
	return false;
}

namespace PlayerBuffer
{
	void TestSeparateBufferPopulation(OpenGLRenderer *openGL)
	{
		int current_buffer = openGL->bufferIDs.size();
		int current_texture = openGL->textureIDs.size();
		int current_vao = openGL->vaoIDs.size();
		openGL->textureIDs.push_back(current_texture);
		openGL->bufferIDs.push_back(current_buffer);
		openGL->vaoIDs.push_back(current_vao);

		glGenVertexArrays(1, &openGL->vaoIDs[current_vao]);
		glBindVertexArray(openGL->vaoIDs[current_vao]);

		// Get a cstyle string for loading the image
		char textureName[] = "resources/test.png";

		// Debug output the name of the texture (make sure stuff isn't broken)
		std::cout << textureName << std::endl;

		// Load the image as a surface (don't need a texture, can be surface for the pixel data)
		SDL_Surface* surface = IMG_Load(textureName);

		// If something bad happened
		if (surface == nullptr)
		{
			std::cout << "Unable to load image " << textureName << "! SDL Error: " << SDL_GetError() << std::endl;
		}

		// Indicate we want to make a new texture
		glGenTextures(1, &openGL->textureIDs[current_texture]);
		// Indicate where this new texture will be bound
		glBindTexture(GL_TEXTURE_2D, openGL->textureIDs[current_texture]);

		// Default to RGB
		int mode = GL_RGB;

		// Otherwise account for alpha channel (we will probably usually have alpha)
		if(surface->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		// Slam in the texture
		glTexImage2D(
			// Target: Here we just say to make it a 2D texture (there are other complicated things for like cubes and stuff)
			GL_TEXTURE_2D,
			// Level of detail (basically for mipmapping an image (shrinking it)
			// 0 here means don't mipmap to reduce it (ie, the base image)
			0,
			// Internal format: how the bytes represent the colors
			mode,
			// Width
			surface->w,
			// Height
			surface->h,
			// The width of the border? Apparently this should *always* be 0
			0,
			// The format for the texels ("3D" texture pixels)
			mode,
			// How exactly the texels should be passed to OpenGL
			GL_UNSIGNED_BYTE,
			// The pixels data itself, we rip it from the surface
			surface->pixels
		);

		// Texture parameters
		// Basically, repeat if you need to and linear interpolation for texel -> pixel
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Contrived for one rectangle
		int numVertices = 6;
		int verticesSize = numVertices * sizeof(vec4);

		// One face of a cube
		vec4 vertices[6] = {
			{-1,  1,  1, 1.0},	// front top left
			{-1, -1,  1, 1.0},	// front bottom left
			{ 1, -1,  1, 1.0},	// front bottom right
			{-1,  1,  1, 1.0},	// front top left
			{ 1, -1,  1, 1.0},	// front bottom right
			{ 1,  1,  1, 1.0},	// front top right
		};

		// **PLEASE NOTE THIS IS UPSIDE DOWN**
		// Why? I think (though I am not sure) that the surface pixels from SDL are upside down
		// That is, (0, 0) is top left from SDL's perspective, HOWEVER (0, 1) is top left from OpenGL's perspective
		vec2 texCoord[6] = {
			{0.0, 0.0},
			{0.0, 0.5},
			{0.5, 0.5},
			{0.0, 0.0},
			{0.5, 0.5},
			{0.5, 0.0},
		};

		// Describes how we will be sending data out to be rendered
		glGenBuffers(1, &openGL->bufferIDs[current_buffer]);
		glBindBuffer(GL_ARRAY_BUFFER, openGL->bufferIDs[current_buffer]);
		// Full buffer
		glBufferData(GL_ARRAY_BUFFER, verticesSize + sizeof(texCoord), NULL, GL_STATIC_DRAW);
		// Vertices
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, vertices);
		// Texture stuff
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, sizeof(texCoord), texCoord);

		// Info for position (vec4 at the moment)
		GLuint vPosition = glGetAttribLocation(openGL->program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Info for the texture (vec2 at the moment)
		GLuint vTexCoord = glGetAttribLocation(openGL->program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) verticesSize);

		// FREE THE SURFACE
		SDL_FreeSurface(surface);
	}
}
