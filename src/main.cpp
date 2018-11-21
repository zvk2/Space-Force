// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "INC_SDL.h"
#include "Magnetar.h"
#include "Player.h"
#include "Enemy.h"
#include "attack.h"
#include "blackhole.h"
#include "Menu.h"
#include <cstdlib>


// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

#include "OpenGLRenderer.hpp"

// Constants for resolution
//~ const int SCREEN_WIDTH = 1280;
//~ const int SCREEN_HEIGHT = 720;

// Constants for level
const int LEVEL_LEN = 5120;
const int TILE_SIZE = 100;

// Constant for acceleration
//const double ACCEL = 3600.0;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

//The context that is used to render OpenGL to the SDL Window
SDL_GLContext gContext;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
int close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gBackground;
SDL_Texture* gAttack;
SDL_Texture* gBlackhole;
SDL_Texture* gPlayerSheet;
SDL_Texture* gHealthbar;
std::vector<SDL_Texture*> gTex;

bool init()
{
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}


	gWindow = SDL_CreateWindow(
		"Space Force",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags)
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}

SDL_Texture* loadImage(std::string fname)
{
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr)
	{
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr)
	{
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

int close()
{
	for (auto i : gTex)
	{
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	SDL_DestroyRenderer(gRenderer);
    SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

	return 0;
}

// CREDITS
int playCredits()
{
	struct dirent *entry;
	DIR *dp;

	dp = opendir(CREDITS_FOLDER);
	if (dp == NULL)
	{
		perror("opendir: Path does not exist or could not be read.");
		return -1;
	}

	while ((entry = readdir(dp)))
	{

		int dNameLength = strlen(entry->d_name);

		// Crude
		if (dNameLength > 4 && entry->d_name[dNameLength - 4] == '.')
		{
			// Crude
			char currentImageBuffer[2000];
			strcpy(currentImageBuffer, CREDITS_FOLDER);
			strcat(currentImageBuffer, entry->d_name);

			// Puts the image on the buffer queue
			gTex.push_back(loadImage(currentImageBuffer));
		}
	}

	// Close the directory
	closedir(dp);

	bool quit = false;
	SDL_Event e;

	for (auto i : gTex)
	{
		// does the user want to quit?
		while(SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		 	if(e.type == SDL_KEYDOWN)
		 	{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
		 	}
		}
		if (quit)
		{
			break;
		}
		// Clear
		SDL_RenderClear(gRenderer);
		// Render the image
		SDL_RenderCopy(gRenderer, i, NULL, NULL);
		// Display rendering
		SDL_RenderPresent(gRenderer);
		// Wait 3 seconds
		SDL_Delay(3000);
	}
	// Clear the renderer one last time
	SDL_RenderClear(gRenderer);

	//close();
    return 0;
}

// Test main
int main() {
	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	// MENU

	//~ std::cout << "Make renderer" << std::endl;
	OpenGLRenderer openGL = OpenGLRenderer(gWindow);

	//~ std::cout << "Make 0s" << std::endl;
	// Just copied
	// Add to openGL.textureIDs (needed to bind texture when rendering)
		//~ openGL.textureIDs.push_back(openGL.textureIDs.size());
		//~ openGL.bufferIDs.push_back(openGL.bufferIDs.size());

		//~ // Get a cstyle string for loading the image
		//~ char textureName[] = "resources/test2.png";

		//~ // Debug output the name of the texture (make sure stuff isn't broken)
		//~ std::cout << textureName << std::endl;

		//~ // Load the image as a surface (don't need a texture, can be surface for the pixel data)
		//~ SDL_Surface* surface = IMG_Load(textureName);

		//~ // If something bad happened
		//~ if (surface == nullptr)
		//~ {
			//~ std::cout << "Unable to load image " << textureName << "! SDL Error: " << SDL_GetError() << std::endl;
		//~ }

		//~ // Indicate we want to make a new texture
		//~ glGenTextures(1, &openGL.textureIDs[0]);
		//~ // Indicate where this new texture will be bound
		//~ glBindTexture(GL_TEXTURE_2D, openGL.textureIDs[0]);

		//~ // Default to RGB
		//~ int mode = GL_RGB;

		//~ // Otherwise account for alpha channel (we will probably usually have alpha)
		//~ if(surface->format->BytesPerPixel == 4) {
			//~ mode = GL_RGBA;
		//~ }

		//~ // Slam in the texture
		//~ glTexImage2D(
			//~ // Target: Here we just say to make it a 2D texture (there are other complicated things for like cubes and stuff)
			//~ GL_TEXTURE_2D,
			//~ // Level of detail (basically for mipmapping an image (shrinking it)
			//~ // 0 here means don't mipmap to reduce it (ie, the base image)
			//~ 0,
			//~ // Internal format: how the bytes represent the colors
			//~ mode,
			//~ // Width
			//~ surface->w,
			//~ // Height
			//~ surface->h,
			//~ // The width of the border? Apparently this should *always* be 0
			//~ 0,
			//~ // The format for the texels ("3D" texture pixels)
			//~ mode,
			//~ // How exactly the texels should be passed to OpenGL
			//~ GL_UNSIGNED_BYTE,
			//~ // The pixels data itself, we rip it from the surface
			//~ surface->pixels
		//~ );

		//~ // Texture parameters
		//~ // Basically, repeat if you need to and linear interpolation for texel -> pixel
		//~ glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//~ glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//~ // Contrived for one rectangle
		//~ int numVertices = 6;
		//~ int verticesSize = numVertices * sizeof(vec4);

		//~ // One face of a cube
		//~ vec4 vertices[6] = {
			//~ {-0.5,  0.5,  0.5, 1.0},	// front top left
			//~ {-0.5, -0.5,  0.5, 1.0},	// front bottom left
			//~ { 0.5, -0.5,  0.5, 1.0},	// front bottom right
			//~ {-0.5,  0.5,  0.5, 1.0},	// front top left
			//~ { 0.5, -0.5,  0.5, 1.0},	// front bottom right
			//~ { 0.5,  0.5,  0.5, 1.0},	// front top right
		//~ };

		//~ // **PLEASE NOTE THIS IS UPSIDE DOWN**
		//~ // Why? I think (though I am not sure) that the surface pixels from SDL are upside down
		//~ // That is, (0, 0) is top left from SDL's perspective, HOWEVER (0, 1) is top left from OpenGL's perspective
		//~ vec2 texCoord[6] = {
			//~ {0.0, 0.0},
			//~ {0.0, 1.0},
			//~ {1.0, 1.0},
			//~ {0.0, 0.0},
			//~ {1.0, 1.0},
			//~ {1.0, 0.0},
		//~ };

		//~ // Describes how we will be sending data out to be rendered
		//~ glGenBuffers(1, &openGL.bufferIDs[0]);
		//~ glBindBuffer(GL_ARRAY_BUFFER, openGL.bufferIDs[0]);
		//~ // Full buffer
		//~ glBufferData(GL_ARRAY_BUFFER, verticesSize + sizeof(texCoord), NULL, GL_STATIC_DRAW);
		//~ // Vertices
		//~ glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, vertices);
		//~ // Texture stuff
		//~ glBufferSubData(GL_ARRAY_BUFFER, verticesSize, sizeof(texCoord), texCoord);

		//~ // Info for position (vec4 at the moment)
		//~ GLuint vPosition = glGetAttribLocation(openGL.program, "vPosition");
		//~ glEnableVertexAttribArray(vPosition);
		//~ glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		//~ // Info for the texture (vec2 at the moment)
		//~ GLuint vTexCoord = glGetAttribLocation(openGL.program, "vTexCoord");
		//~ glEnableVertexAttribArray(vTexCoord);
		//~ glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) verticesSize);

		//~ // FREE THE SURFACE
		//~ SDL_FreeSurface(surface);

	PlayerBuffer::TestSeparateBufferPopulation(&openGL);
	EnemyBuffer::TestSeparateBufferPopulation(&openGL);

	// Rough sketch of a RenderObject?
	RenderObject test = RenderObject(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 6, 0, 0
	);

	RenderObject test2 = RenderObject(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 6, 1, 1
	);

	// Crude idea of how to add to render queue?
	openGL.AppendRenderObject(test);
	openGL.AppendRenderObject(test2);

	// Displays stuff
	openGL.Display();

	bool loop = 1;
	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				case SDLK_q:
					loop = false;
					break;
				default:
					break;
				}
			}
		}
	}

	openGL.Close();
	return -1;
}

//~ int main(int argc, char* argv[])
//~ {
	//~ if (!init())
	//~ {
		//~ std::cout <<  "Failed to initialize!" << std::endl;
		//~ close();
		//~ return 1;
	//~ }
	//~ // MENU

	//~ Menu menu;
	//~ menu.displayMenu(gWindow);
	//~ int selection = menu.runMenu(gWindow);
	//~ //std::cout << selection << std::endl;
	//~ while (selection == 2)
	//~ {
		//~ playCredits();
		//~ selection = menu.runMenu(gWindow);
	//~ }
	//~ if (selection == 0)
	//~ {
		//~ close();
		//~ return 0;
	//~ }
	//~ if (selection == 1)
	//~ {
        //~ gContext = menu.closeMenu();
	//~ }


	//~ // GAME
	//~ /*
	//~ - Controls: WASD for movement, Spacebar to shoot
	//~ - Can be terminated by x'ing out or pressing 'esc' to credits
	//~ - Starman sprite's dimensions are 300 x 51
	//~ */
	//~ gBackground = loadImage("resources/imgs/space_2_background.png");
	//~ gAttack = loadImage("resources/imgs/attack.png");
    	//~ gBlackhole = loadImage("resources/imgs/blackhole.png");
	//~ gHealthbar = loadImage("resources/imgs/healthbar.png");



	//~ int scrollOffset = 0;
	//~ int rem = 0;
	//~ double xDeltav = 0.0;
	//~ double yDeltav = 0.0;
	//~ bool create;

	//~ SDL_RendererFlip flip = SDL_FLIP_NONE;

	//~ int frames = 0;
	//~ int frameCount = 0;

	//~ SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	//~ Uint32 fpsLasttime = SDL_GetTicks();
	//~ Uint32 fpsCurtime = 0;
	//~ Uint32 moveLasttime = SDL_GetTicks();
	//~ double timestep = 0;
	//~ SDL_Rect attackRect = {0, 0, 60, 10};
	//~ //SDL_Rect attackCam = {SCREEN_WIDTH+80, SCREEN_HEIGHT/2+51/2, 80, 20};
	//~ SDL_Rect blackholeRect = {0, 0, 300, 300};
	//~ SDL_Rect blackholeCam = {SCREEN_WIDTH,SCREEN_HEIGHT/2, 300, 300};
	//~ Player ply(10, loadImage("resources/imgs/starman.png"), 1,gRenderer);

	//~ SDL_Rect healthRect = {0, 0, 177, 33};
	//~ SDL_Rect healthCam = {30, 30, 177, 33};

	//~ Magnetar mag(&ply, loadImage("resources/imgs/Magnetars.png"));
	//~ double ACCEL = ply.GetMove();

  //~ Enemy emy(10, loadImage("resources/imgs/faxanaduitis.png"), 1);
  //~ emy.setPosition(860, 0);
	//~ emy.setVelocity(0, 50);

	//~ //the beginning/default image and attack box
	//~ ply.hit.setAttack(gAttack,&attackRect);
	//~ SDL_Event e;
	//~ bool gameOn = true;
	//~ bool up = true;
	//~ bool credits = true;
    //~ double emyDelta = 1;

	//~ while(gameOn)
	//~ {

		//~ while(SDL_PollEvent(&e))
		//~ {
			//~ if (e.type == SDL_QUIT)
			//~ {
				//~ gameOn = false;
				//~ credits = false;
			//~ }
			//~ if (e.type == SDL_KEYDOWN)
			//~ {
				//~ if (e.key.keysym.sym == SDLK_ESCAPE)
				//~ {
					//~ gameOn = false;
					//~ credits = false;
				//~ }
				//~ if (e.key.keysym.sym == SDLK_q)
				//~ {
					//~ gameOn = false;
				//~ }
			//~ }
			//~ if(up == false && e.type == SDL_KEYUP && e.key.repeat == 0)
			//~ {
				//~ if(e.key.keysym.sym == SDLK_SPACE)
				//~ {
					//~ up = true;
				//~ }
			//~ }
		//~ }
		//~ ACCEL = ply.GetMove();
		//~ timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;
		//~ xDeltav = 0.0;
		//~ yDeltav = 0.0;

		//~ // WASD movement
		//~ const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		//~ if (keyState[SDL_SCANCODE_A])
			//~ xDeltav -= (ACCEL * timestep);
		//~ if (keyState[SDL_SCANCODE_D])
			//~ xDeltav += (ACCEL * timestep);
		//~ if (keyState[SDL_SCANCODE_W])
			//~ yDeltav -= (ACCEL * timestep);
		//~ if (keyState[SDL_SCANCODE_S])
			//~ yDeltav += (ACCEL * timestep);

		//~ if (emy.getEnemyCam().y + emy.getEnemyCam().h == SCREEN_HEIGHT)
		//~ {
			//~ emyDelta = -1;
			//~ emy.setVelocity(0, -10);
		//~ }
		//~ if (emy.getEnemyCam().y == 0)
		//~ {
			//~ emyDelta = 1;
			//~ emy.setVelocity(0, 10);
		//~ }

		//~ SDL_Rect pRect = ply.getPlayerRect();
		//~ SDL_Rect pCam = ply.getPlayerCam();

		//~ SDL_Rect eRect = emy.getEnemyRect();
		//~ SDL_Rect eCam = emy.getEnemyCam();

		//~ moveLasttime = SDL_GetTicks();

		//~ // Scrolling background
		//~ ++scrollOffset;
		//~ if (scrollOffset > bgRect.w)
		//~ {
			//~ scrollOffset = 0;
		//~ }

		//~ SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		//~ SDL_RenderClear(gRenderer);

		//~ rem = scrollOffset % SCREEN_WIDTH;
		//~ bgRect.x = -rem;


		//~ SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);
		//~ bgRect.x += SCREEN_WIDTH;
		//~ SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);

		//~ frames += 1 % 1000000000;

		//~ ply.animate(frames);
		//~ emy.animate(frames);


		//~ // Since game levels progress from L to R, no need for sprite to flip
		//~ // Code for flipping remains here if theres a change of plan

		//~ // Flip if facing other direction
		//~ /*if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			//~ flip = SDL_FLIP_NONE;
		//~ else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			//~ flip = SDL_FLIP_HORIZONTAL;*/

		//~ pRect = ply.getPlayerRect();
		//~ pCam = ply.getPlayerCam();
        //~ Uint32 currTime = SDL_GetTicks();
        //~ if(currTime>=6000)
		//~ {
            //~ //std::cout << currTime % 3000 << std::endl;
			//~ if((currTime % 3000 <= 50 && !mag.Seen()) ||mag.Seen())
			//~ {

				//~ mag.Render();
			//~ }
		//~ }
        //~ if(currTime >= 5000)
        //~ {
            //~ int bFrames;
            //~ if(currTime % 5000 == 0)
            //~ {
                //~ SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
                //~ bFrames = 0;
                //~ //blackhole vacuum(gRenderer,gBlackhole,&blackholeRect,blackholeCam);
            //~ }
            //~ else
            //~ {
                //~ bFrames++;

                //~ if (bFrames / 12 > 5)
                    //~ bFrames = 0;

                //~ blackholeRect.x = (bFrames / 12) * 300;
                //~ blackholeCam.x = blackholeCam.x - 1;
                //~ SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);

                //~ if(blackholeCam.x < SCREEN_WIDTH && blackholeCam.x > 0)
                //~ {
                    //~ if(blackholeCam.x + 150 > pCam.x)
                    //~ {
                        //~ if(blackholeCam.y + 150 > pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav + 20;
                        //~ }
                        //~ if(blackholeCam.y + 150 < pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav - 20;
                        //~ }
                        //~ xDeltav = xDeltav + 20;
                    //~ }
                    //~ else if(blackholeCam.x + 150 < pCam.x)
                    //~ {
                        //~ if(blackholeCam.y + 150 > pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav + 20;
                        //~ }
                        //~ if(blackholeCam.y + 150 < pCam.y)
                        //~ {
                            //~ yDeltav = yDeltav - 20;
                        //~ }
                        //~ xDeltav = xDeltav - 20;
                    //~ }
                //~ }

            //~ }

            //~ if(blackholeCam.x == -300)
            //~ {
                //~ blackholeCam = {SCREEN_WIDTH,rand() % (SCREEN_HEIGHT-300), 300, 300};
                //~ bFrames = 0;
            //~ }

        //~ }

		//~ ply.move(xDeltav, yDeltav, timestep);
		//~ bool collision = ply.checkEnemyCollision(&emy, timestep);

		//~ if (collision)
		//~ {
			//~ //ply.LostHealth(1);
			//~ if (healthRect.x == 1770)
			//~ {
				//~ return playCredits();
			//~ }
			//~ else
			//~ {
				//~ healthRect.x += 177;
			//~ }
		//~ }

		//~ emy.move(0, emyDelta, timestep);
		//~ emy.checkPlayerCollision(&ply, timestep);
		//~ /*
		//~ collision = emy.checkPlayerCollision(&ply, timestep);
		//~ if (collision)
		//~ {
			//~ //Minor enemies should be destroyed in event of collision
		//~ }
		//~ */
		//~ pCam = ply.getPlayerCam();
		//~ eCam = emy.getEnemyCam();

        //~ //attack button

		//~ if(keyState[SDL_SCANCODE_SPACE] && up == true)
		//~ {
			//~ up = false;
			//~ ply.hit.addAttack(pCam.x + 300,pCam.y + 51/2);
		//~ }
		//~ //lets the attack move across the screen
		//~ ply.hit.renderAttack(timestep);
		//~ SDL_RenderCopyEx(gRenderer, ply.getPlayerSheet(), &pRect, &pCam, 0.0, nullptr, flip);
		//~ SDL_RenderCopyEx(gRenderer, emy.getEnemySheet(), &eRect, &eCam, 0.0, nullptr, flip);

		//~ SDL_RenderCopy(gRenderer, gHealthbar, &healthRect, &healthCam);

		//~ SDL_RenderPresent(gRenderer);

	//~ }
	//~ if (credits)
	//~ {
		//~ return playCredits();
	//~ }
	//~ close();
	//~ //return credits ? playCredits() : close();
//~ }
