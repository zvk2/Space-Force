
#include "INC_SDL.h"
#include "Enemy.h"
#include <cmath>
#define MAX_SPEED 50

//Public methods
Enemy::Enemy(int startingHealth, SDL_Texture* characterImages, int attack):
	hitPoints(startingHealth), enemySheet(characterImages),
	attackPower(attack), phys(0, 0, 300.0, 3600.0), xCoord(1280/8), yCoord(720/2)
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

	enemyCam.x = (int) xCoord;
	enemyCam.y = (int) yCoord;
}

// Animate jet propulsion
void Enemy::animate(int frames)
{
	enemyRect.x = ((frames / 10) % 4) * enemyRect.w;
}

//Check for collision with the player
void Enemy::checkPlayerCollision(Player* p, double tstep)
{
	SDL_Rect pRect = p->getPlayerCam();

	if (SDL_HasIntersection(&pRect, &enemyCam))
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

namespace EnemyBuffer
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
		char textureName[] = "resources/test2.png";

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
			{-0.5,  0.5,  0.5, 1.0},	// front top left
			{-0.5, -0.5,  0.5, 1.0},	// front bottom left
			{ 0.5, -0.5,  0.5, 1.0},	// front bottom right
			{-0.5,  0.5,  0.5, 1.0},	// front top left
			{ 0.5, -0.5,  0.5, 1.0},	// front bottom right
			{ 0.5,  0.5,  0.5, 1.0},	// front top right
		};

		// **PLEASE NOTE THIS IS UPSIDE DOWN**
		// Why? I think (though I am not sure) that the surface pixels from SDL are upside down
		// That is, (0, 0) is top left from SDL's perspective, HOWEVER (0, 1) is top left from OpenGL's perspective
		//~ vec2 texCoord[6] = {
			//~ {0.0, 0.0},
			//~ {0.0, 1.0},
			//~ {1.0, 1.0},
			//~ {0.0, 0.0},
			//~ {1.0, 1.0},
			//~ {1.0, 0.0},
		//~ };

		vec2 texCoord[6] = {
			{0.5, 0.0},
			{0.5, 1.0},
			{1.0, 1.0},
			{0.5, 0.0},
			{1.0, 1.0},
			{1.0, 0.0},
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
