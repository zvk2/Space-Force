// TODO INCLUDES
// Includes
#ifndef OPENGLRENDERER_HPP
#define OPENGLRENDERER_HPP
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
		RenderObject(GLfloat initX, GLfloat initY, GLfloat initZ, GLfloat initWidth, GLfloat initHeight, int numVertices, GLuint initTextureID, GLuint initBufferID);
		//~ RenderObject(GLfloat initX, GLfloat initY, GLfloat initWidth, GLfloat initHeight, int numVertices, GLuint initTextureID, GLuint initBufferID);
		~RenderObject();
		void ChangeCoordinates(GLfloat newX, GLfloat newY);
	// Bad taste to make these public
	// Also probably better to have this just be a stuct or something?
	//~ private:
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat width;
		GLfloat height;
		// I theoretically *could* have a mapping from bufferID to numVertices or something, but I don't know...
		int numVertices;
		GLuint textureID;
		GLuint bufferID;
		// bufferID should be the same as the vaoID
		// GLuint vaoID;
		// Transformation matrix?
		mat4 ctm;
		// Index in renderObjects (in OpenGLRenderer)
		int index;
};

// Need to think about how to integrate this class with other entity classes
//~ class RenderObject
//~ {
	//~ public:
		//~ int index;
		//~ int current_buffer_index;
		//~ int final_buffer_index;
		//~ int numVertices;
		//~ mat4 transformation_matrix;
//~ };

class OpenGLRenderer
{
	public:
		// Constructor
		OpenGLRenderer(SDL_Window*);
		// Destructor
		//~ ~OpenGLRenderer();
		void Close();
		//~ int CreateWindow();
		// CONTRIVED AT THE MOMENT
		void PopulateTextures();
		void AppendRenderObject(RenderObject *newRenderObject);
		void Display();
	//~ private:
		// Number of vertices (needed to render)
		int number_of_vertices;
		// The camera
		mat4 frame;
		// The projection (frustum, etc)
		mat4 projection;
		// An array of objects to render
		std::vector<RenderObject*> renderObjects;
		// Vector of textures
		std::vector<GLuint> textureIDs;
		// Vector of buffers
		std::vector<GLuint> bufferIDs;
		// Vector of vertex arrays
		std::vector<GLuint> vaoIDs;
		// TODO
		// The window to display to
		SDL_Window* gWindow;
		// The opengl context handle
		SDL_GLContext mainContext;
		// CTM Location
		GLuint ctmLocation;
		// The "program" (shaders) used
		GLuint program;
};

GLfloat CanonicalCoordinatesFromPixels(int pixels, int dimension);

void PopulateDefault2DBuffer(OpenGLRenderer* openGL, char *file_name, int width, int height);
void PopulateDefault2DBuffers(OpenGLRenderer* openGL, char *file_name, int width, int height, int rows, int columns);
#endif
