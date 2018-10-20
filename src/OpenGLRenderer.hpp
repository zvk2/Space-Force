// TODO INCLUDES
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

// Physics
//~ #include "physics.hpp"

// For type definitions etc.
#include "matrix.h"

// For getting the shaders
#include "shader.h"


#ifdef CreateWindow
#undef CreateWindow
#endif

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Need to think about how to integrate this class with other entity classes
class RenderObject
{
	public:
		// Constructor
		RenderObject(double initX, double initY, double initWidth, double initHeight, GLuint initTextureID);
		~RenderObject();
	// Bad taste to make these public
	// Also probably better to have this just be a stuct or something?
	//~ private:
		double x;
		double y;
		double width;
		double height;
		GLuint textureID;
};

class OpenGLRenderer
{
	public:
		// Constructor
		OpenGLRenderer();
		// Destructor
		//~ ~OpenGLRenderer();
		void Close();
		int CreateWindow();
		// CONTRIVED AT THE MOMENT
		void PopulateTextures();
		void AppendRenderObject(RenderObject newRenderObject);
		void Display();
	private:
		// Number of vertices (needed to render)
		int number_of_vertices;
		// The camera
		mat4 frame;
		// The projection (frustum, etc)
		mat4 projection;
		// An array of objects to render
		std::vector<RenderObject> renderObjects;
		// Array of textures
		std::vector<GLuint> textureIDs;
		// Array of buffers
		std::vector<GLuint> bufferIDs;
		// TODO
		// The window to display to
		SDL_Window* gWindow;
		// The opengl context handle
		SDL_GLContext mainContext;
		// The "program" (shaders) used
		GLuint program;
};
