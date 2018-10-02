// This file iterates over every listed file in the folder "Credit_Image" and displays it for 3 seconds
// This uses "sdl0_hello.cpp" as boilerplate

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

// For getting the shaders
#include "initShader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Parent folder for credit images
// Not const due to contrivance (can pass immediately if not const)
char CREDITS_FOLDER[] = "resources/Credit_Image/";

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

// The opengl context handle
SDL_GLContext mainContext;

// CRUDE JUST FOR TESTING
typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3;

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

vec3 vertices[36] = {
	// Face 1
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	// Face 2
	{0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{0.5, -0.5, 0.5},
	// Face 3
	{0.5, -0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, -0.5, -0.5},
	// Face 4
	{-0.5, -0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, -0.5, -0.5},
	{-0.5, 0.5, -0.5},
	// Face 5
	{0.5, 0.5, -0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{0.5, 0.5, -0.5},
	// Face 6
	{0.5, -0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
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

GLuint ctm_location;
mat4 identity_matrix = 
{{1.0, 0.0, 0.0, 0.0},
 {0.0, 1.0, 0.0, 0.0},
 {0.0, 0.0, 1.0, 0.0},
 {0.0, 0.0, 0.0, 1.0}};

mat4 tr_matrix =
{{1.0, 0.0, 0.0, 0.0},
 {0.0, 1.0, 0.0, 0.0},
 {0.0, 0.0, 1.0, 0.0},
 {0.0, 0.0, 0.0, 1.0}};

int enableIdle = 0;
int leftDown = 1;

mat4 frustum(
    GLfloat left, GLfloat right,
    GLfloat bottom, GLfloat top,
    GLfloat near, GLfloat far
)
{
    mat4 frustum = {{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}};
    
    frustum.x.x = (-2.0f * near) / (right - left);
    frustum.y.y = (-2.0f * near) / (top - bottom);
    frustum.z.x = (left + right) / (right - left);
    frustum.z.y = (bottom + top) / (top - bottom);
    frustum.z.z = (near + far) / (far - near);
    frustum.z.w = -1.0f;
    frustum.w.z = -1.0f*(2.0f * near * far) / (far - near);
    
    return frustum;
}

void mat4_multiplication(mat4 *m1, mat4 *m2, mat4 *result) {
    result->x.x = m1->x.x * m2->x.x + m1->y.x * m2->x.y + m1->z.x * m2->x.z + m1->w.x * m2->x.w;
    result->x.y = m1->x.y * m2->x.x + m1->y.y * m2->x.y + m1->z.y * m2->x.z + m1->w.y * m2->x.w;
    result->x.z = m1->x.z * m2->x.x + m1->y.z * m2->x.y + m1->z.z * m2->x.z + m1->w.z * m2->x.w;
    result->x.w = m1->x.w * m2->x.x + m1->y.w * m2->x.y + m1->z.w * m2->x.z + m1->w.w * m2->x.w;
    result->y.x = m1->x.x * m2->y.x + m1->y.x * m2->y.y + m1->z.x * m2->y.z + m1->w.x * m2->y.w;
    result->y.y = m1->x.y * m2->y.x + m1->y.y * m2->y.y + m1->z.y * m2->y.z + m1->w.y * m2->y.w;
    result->y.z = m1->x.z * m2->y.x + m1->y.z * m2->y.y + m1->z.z * m2->y.z + m1->w.z * m2->y.w;
    result->y.w = m1->x.w * m2->y.x + m1->y.w * m2->y.y + m1->z.w * m2->y.z + m1->w.w * m2->y.w;
    result->z.x = m1->x.x * m2->z.x + m1->y.x * m2->z.y + m1->z.x * m2->z.z + m1->w.x * m2->z.w;
    result->z.y = m1->x.y * m2->z.x + m1->y.y * m2->z.y + m1->z.y * m2->z.z + m1->w.y * m2->z.w;
    result->z.z = m1->x.z * m2->z.x + m1->y.z * m2->z.y + m1->z.z * m2->z.z + m1->w.z * m2->z.w;
    result->z.w = m1->x.w * m2->z.x + m1->y.w * m2->z.y + m1->z.w * m2->z.z + m1->w.w * m2->z.w;
    result->w.x = m1->x.x * m2->w.x + m1->y.x * m2->w.y + m1->z.x * m2->w.z + m1->w.x * m2->w.w;
    result->w.y = m1->x.y * m2->w.x + m1->y.y * m2->w.y + m1->z.y * m2->w.z + m1->w.y * m2->w.w;
    result->w.z = m1->x.z * m2->w.x + m1->y.z * m2->w.y + m1->z.z * m2->w.z + m1->w.z * m2->w.w;
    result->w.w = m1->x.w * m2->w.x + m1->y.w * m2->w.y + m1->z.w * m2->w.z + m1->w.w * m2->w.w;
}

mat4 viewing_frustum = frustum(
    -1, 1,
    -1, 1,
    -1, 1
);


bool init()
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
		"Space Force Credits",
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
	GLuint program = initShader("src/vshader.glsl", "src/fshader.glsl");
    glUseProgram(program);

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
    //transformation_matrix = m4_identity();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
    
    // TODO TODO TODO BE SURE CHANGE THE ASPECT RATIO WITH A VIEWING FRUSTUM
	
	return true;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //~ mat4 temp;
    //~ mat4_multiplication(&viewing_frustum, &tr_matrix, &temp);
    
    //~ mat4_multiplication(&identity_matrix, &temp, &tr_matrix);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);

    glPolygonMode(GL_FRONT, GL_FILL);
    //~ glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    SDL_GL_SwapWindow(gWindow);
}

void close()
{
	
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(gWindow);
	
	bool loop = true;

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
		
		display();
	}

	close();
}
