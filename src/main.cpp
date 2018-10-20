// This file iterates over every listed file in the folder "Credit_Image" and displays it for 3 seconds
// This uses "sdl0_hello.cpp" as boilerplate
// EXAMPLE: clang++ -I/usr/include/SDL2 -o bin/test3d.exe src/main.cpp src/initShader.c -lSDL2 -lGLEW -lglut -lGL -lGLU -lm

// Includes
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// checking for apple, else everyone else
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// OpenGL / glew Headers
// ONLY TESTED ON LINUX
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

// Physics
#include "physics.hpp"

// For getting the shaders
#include "shader.h"

// TO BE REVISED: helperfunctions
#include "helperFunctions.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

// Function declarations
bool InitOpenGl();
bool InitSdlRenderer();
int playCredits();
SDL_Texture* loadImage(std::string fname);
void CloseOpenGl();
int CloseProgram();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

// The opengl context handle
SDL_GLContext mainContext;

mat4 projection = frustum(
	-1.7778, 1.7778,
	-1, 1,
	-1, -2000
);

mat4 frame = look_at(
	0, 0, 3,
	0, 0, -100,
	0, 1, 0
);

GLuint ctm_location;
GLuint projection_matrix_location;
GLuint model_view_matrix_location;

mat4 constant_xrot;
mat4 constant_yrot;
mat4 constant_zrot;

mat4 identity =
{
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

mat4 tr =
{
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

mat4 ctm =
{
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};

mat4 rot =
{
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};
 
vec3 vertices[36] = {
	// Face 1
	{0.5, 0.5, -0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{0.5, -0.5, -0.5},
	// Face 2
	{-0.5, 0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	// Face 3
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, 0.5, 0.5},
	{0.5, -0.5, -0.5},
	// Face 4
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	// Face 5
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	// Face 6
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
};

vec4 colors[36] = {
	{1.0, 0, 0, 0},
	{1.0, 0, 0, 0},
	{1.0, 0, 0, 0},
	{1.0, 0, 0, 0},
	{1.0, 0, 0, 0},
	{1.0, 0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 1.0, 0, 0},
	{0, 0, 1.0, 0},
	{0, 0, 1.0, 0},
	{0, 0, 1.0, 0},
	{0, 0, 1.0, 0},
	{0, 0, 1.0, 0},
	{0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{1.0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
	{0, 1.0, 1.0, 0},
};

int num_vertices = 36;


int CloseProgram()
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
	
	return -1;
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


bool InitSdlRenderer()
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
		"Space Force Credits",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
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
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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


// Display the credits.
int playCredits()
{
	if (!InitSdlRenderer())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		CloseProgram();
		return 1;
	}
	
	SDL_RenderClear(gRenderer);
	// Load media by iterating over the files in "Credit_Image"	
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
	
	for (auto i : gTex)
	{
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
	return CloseProgram();
}


bool InitOpenGl()
{
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	// Enable double buffering for OpenGL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	
	gWindow = SDL_CreateWindow(
		"XXXtreme Peace King: Coronation MEGAMIX",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		// Note change here
		SDL_WINDOW_OPENGL
	);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}
	
	// Set the main context for opengl
	mainContext = SDL_GL_CreateContext(gWindow);
	
	// Set opengl attributes here!
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	// HOWEVER, I think we should 3.0 to avoid compatability issues
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//~ SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	// Sync buffer swap with monitor vertical refresh (attempt to avoid flicker/tear)
	SDL_GL_SetSwapInterval(1);
	
	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me (headerphile) know
	// (Also thanks in turn to headerphile for the boiler-plate [Luke])
	#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
	#endif
	
	// INIT STUFF SPECIFIC TO OPENGL
	Shader shader("ztest");
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
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

	ctm_location = glGetUniformLocation(program, "ctm");
	projection_matrix_location = glGetUniformLocation(program, "projection");
	model_view_matrix_location = glGetUniformLocation(program, "model_view");

	glEnable(GL_DEPTH_TEST);
	// If you want black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthRange(1,0);
	
	return true;
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// I can reverse the order to change the effect from rotation in place to rotation about axis!
	mat4_multiplication(&tr, &rot, &ctm);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, (GLfloat *) &projection);
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &frame);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
	glEnable(GL_CULL_FACE);

	SDL_GL_SwapWindow(gWindow);
}


void CloseOpenGl()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;
}

int main(int argc, char* argv[])
{
	if (!InitOpenGl())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		CloseProgram();
		return 1;
	}
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(gWindow);
	
	get_rotation_matrix_about_x(PI/360, &constant_xrot);
	get_rotation_matrix_about_y(PI/270, &constant_yrot);
	get_rotation_matrix_about_z(PI/180, &constant_zrot);
	
	bool loop = true;
	bool kill = false;

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
					kill = true;
					break;
				case SDLK_q:
					loop = false;
					break;
				// Up
				case SDLK_w:
					tr.w.y += 0.1;
					break;
				// Down
				case SDLK_s:
					tr.w.y -= 0.1;
					break;
				// Left
				case SDLK_a:
					tr.w.x -= 0.1;
					break;
				// Right
				case SDLK_d:
					tr.w.x += 0.1;
					break;
				// zoom in
				case SDLK_i:
					tr.w.z += 0.3;
					break;
				// zoom out
				case SDLK_k:
					tr.w.z -= 0.3;
					break;
				// zoom in
				case SDLK_KP_8:
					tr.w.z += 0.3;
					break;
				// zoom out
				case SDLK_KP_2:
					tr.w.z -= 0.3;
					break;
				default:
					printf("%d\n", event.key.keysym.sym);
					break;
				}
			}
		}
		
		mat4 interim1;
		mat4 interim2;
		mat4 interim3;
		mat4_multiplication(&constant_xrot, &rot, &interim1);
		mat4_multiplication(&constant_yrot, &interim1, &interim2);
		
		//~ mat4_multiplication(&constant_zrot, &interim2, &interim3);
		//~ mat4_multiplication(&identity, &interim3, &rot);
		
		mat4_multiplication(&identity, &interim2, &rot);
		
		display();
	}
	
	CloseOpenGl();
	
	if (kill) {
		CloseProgram();
	}
	else {
		return playCredits();
	}
}
