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

#include "GL/glew.h"
//#include "matrix.h"
#include "shader.h"

#include <cstdlib>


// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Constants for level
const int LEVEL_LEN = 5120;
const int TILE_SIZE = 100;


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

vec4 vertices[6] =
{{ 0.0,  0.5,  0.0, 1.0},	// top
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 { 0.5,  0.8, -0.5, 1.0},	// top
 { 0.9,  0.0, -0.5, 1.0},	// bottom right
 { 0.1,  0.0, -0.5, 1.0}};	// bottom left

vec4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},	// red   (for top)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom left)
 {0.0, 0.0, 1.0, 1.0},	// blue  (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0},	// green (for bottom right)
 {0.0, 1.0, 0.0, 1.0}};	// green (for bottom right)

int num_vertices = 6;

// Constant for acceleration
//const double ACCEL = 3600.0;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

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
SDL_GLContext gContext;
std::vector<SDL_Texture*> gTex;

void glInit(void)
{
    Shader shader("tritest");
    GLuint program = shader.getProgram();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    SDL_GL_SwapWindow(gWindow);
}

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


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// create our window and OpenGL context
	gWindow = SDL_CreateWindow(
		"Space Force",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	gContext = SDL_GL_CreateContext(gWindow);

	// initiate glew, and force modern OpenGL with experimental
	glewExperimental = GL_TRUE;
	glewInit();
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

	close();
    return 0;
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	glInit();

	// GAME
	/*
	- Uses modified FINAL_sdl15_fps.cpp as basis
	- Currently consists of Starman moving across a scrolling background repeated 4 times
	- Controls: WASD for movement, Spacebar to shoot
	- Can be terminated by x'ing out or pressing 'esc' to credits
	- Starman sprite's dimensions are 240 x 51
	*/
	// Until we figure out gradients, we'll use space_2_background for now
	gBackground = loadImage("resources/imgs/space_2_background.png");
	gAttack = loadImage("resources/imgs/attack.png");
    gBlackhole = loadImage("resources/imgs/blackhole.png");

	int scrollOffset = 0;
	int rem = 0;
	double xDeltav = 0.0;
	double yDeltav = 0.0;
	bool create;
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	int frames = 0;
	int frameCount = 0;

	SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	Uint32 fpsLasttime = SDL_GetTicks();
	Uint32 fpsCurtime = 0;
	Uint32 moveLasttime = SDL_GetTicks();
	double timestep = 0;
	SDL_Rect attackRect = {0, 0, 80, 20};
	//SDL_Rect attackCam = {SCREEN_WIDTH+80, SCREEN_HEIGHT/2+51/2, 80, 20};
  SDL_Rect blackholeRect = {0, 0, 300, 300};
  SDL_Rect blackholeCam = {SCREEN_WIDTH,SCREEN_HEIGHT/2, 300, 300};
	Player ply(10, loadImage("resources/imgs/starman.png"), 1,gRenderer);

	Magnetar mag(&ply, loadImage("resources/imgs/Magnetars.png"));
	double ACCEL = ply.GetMove();

  Enemy emy(10, loadImage("resources/imgs/faxanaduitis.png"), 1);
  emy.setPosition(860, 0);
	emy.setVelocity(0, 50);

	//the beginning/default image and attack box
	ply.hit.setAttack(gAttack,&attackRect);
	SDL_Event e;
	bool gameOn = true;
	bool up = true;
	bool credits = true;
    double emyDelta = 1;
  
	while(gameOn)
	{
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameOn = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					gameOn = false;
					credits = false;
				}
				if (e.key.keysym.sym == SDLK_q)
				{
					gameOn = false;
				}
			}
			if(up == false && e.type == SDL_KEYUP && e.key.repeat == 0)
			{
				if(e.key.keysym.sym == SDLK_SPACE)
				{
					up = true;
				}
			}
		}
		ACCEL = ply.GetMove();
		timestep = (SDL_GetTicks() - moveLasttime) / 1000.0;
		xDeltav = 0.0;
		yDeltav = 0.0;

		// WASD movement
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_A])
			xDeltav -= (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_D])
			xDeltav += (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_W])
			yDeltav -= (ACCEL * timestep);
		if (keyState[SDL_SCANCODE_S])
			yDeltav += (ACCEL * timestep);
		
		if (emy.getEnemyCam().y + emy.getEnemyCam().h == SCREEN_HEIGHT)
		{
			emyDelta = -1;
			emy.setVelocity(0, -10);
		}
		if (emy.getEnemyCam().y == 0)
		{
			emyDelta = 1;
			emy.setVelocity(0, 10);
		}
		
		ply.move(xDeltav, yDeltav, timestep);
		emy.move(0, emyDelta, timestep);
		
		SDL_Rect pRect = ply.getPlayerRect();
		SDL_Rect pCam = ply.getPlayerCam();
		
		SDL_Rect eRect = emy.getEnemyRect();
		SDL_Rect eCam = emy.getEnemyCam();
		
		moveLasttime = SDL_GetTicks();
        
		// Scrolling background
		++scrollOffset;
		if (scrollOffset > bgRect.w)
		{
			scrollOffset = 0;
		}

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		rem = scrollOffset % SCREEN_WIDTH;
		bgRect.x = -rem;
		
		
		SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);
		bgRect.x += SCREEN_WIDTH;
		SDL_RenderCopy(gRenderer, gBackground, nullptr, &bgRect);
		
		frames += 1 % 1000000000;

		ply.animate(frames);
		emy.animate(frames);
		
	
		// Since game levels progress from L to R, no need for sprite to flip
		// Code for flipping remains here if theres a change of plan

		// Flip if facing other direction
		/*if (ply.getxVel() > 0 && flip == SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_NONE;
		else if (ply.getxVel() < 0 && flip == SDL_FLIP_NONE)
			flip = SDL_FLIP_HORIZONTAL;*/

		pRect = ply.getPlayerRect();
		pCam = ply.getPlayerCam();
        Uint32 currTime = SDL_GetTicks();
        if(currTime>=6000)
		{
            //std::cout << currTime % 3000 << std::endl;
			if((currTime % 3000 <= 50 && !mag.Seen()) ||mag.Seen())
			{
				
				mag.Render();
			}
		}
        if(currTime >= 5000)
        {
            int bFrames;
            if(currTime % 5000 == 0)
            {
                SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
                bFrames = 0;
                //blackhole vacuum(gRenderer,gBlackhole,&blackholeRect,blackholeCam);
            }
            else
            {
                bFrames++;

                if (bFrames / 12 > 5)
                    bFrames = 0;
                
                blackholeRect.x = (bFrames / 12) * 300;
                
//                std::cout << "\n";
//                std::cout << "\n";
//                std::cout << "bFrames = " << bFrames;
//                std::cout << "\n";
//                std::cout << "bFrames / 12 = " << bFrames/12;
//                std::cout << "\n";
//                std::cout << "\n";

                blackholeCam.x = blackholeCam.x - 1;
                SDL_RenderCopy(gRenderer, gBlackhole, &blackholeRect, &blackholeCam);
                
                if(blackholeCam.x < SCREEN_WIDTH && blackholeCam.x > 0)
                {
                    if(blackholeCam.x + 150 > pCam.x)
                    {
                        if(blackholeCam.y + 150 > pCam.y)
                        {
                            yDeltav = yDeltav + 20;
                        }
                        if(blackholeCam.y + 150 < pCam.y)
                        {
                            yDeltav = yDeltav - 20;
                        }
                        xDeltav = xDeltav + 20;
                        ply.move(xDeltav, yDeltav, timestep);
                    }
                    else if(blackholeCam.x + 150 < pCam.x)
                    {
                        if(blackholeCam.y + 150 > pCam.y)
                        {
                            yDeltav = yDeltav + 20;
                        }
                        if(blackholeCam.y + 150 < pCam.y)
                        {
                            yDeltav = yDeltav - 20;
                        }
                        xDeltav = xDeltav - 20;
                        ply.move(xDeltav, yDeltav, timestep);
                    }
                }
                
            }
            
            if(blackholeCam.x == -300)
            {
                blackholeCam = {SCREEN_WIDTH,rand() % (SCREEN_HEIGHT-300), 300, 300};
                bFrames = 0;
            }

        }
//        
//        std::cout << "\n";
//        std::cout << "ply.x = " << ply.getPlayerCam().x;
//        std::cout << "\n";
        //attack button 

		if(keyState[SDL_SCANCODE_SPACE] && up == true)
		{
			up = false;
			ply.hit.addAttack(pCam.x + 300,pCam.y + 51/2);
		}
		//lets the attack move across the screen
		ply.hit.renderAttack(timestep);
		display();
		SDL_RenderCopyEx(gRenderer, ply.getPlayerSheet(), &pRect, &pCam, 0.0, nullptr, flip);
		SDL_RenderCopyEx(gRenderer, emy.getEnemySheet(), &eRect, &eCam, 0.0, nullptr, flip);
		SDL_RenderPresent(gRenderer);
	}

	return credits ? playCredits() : close();
}
