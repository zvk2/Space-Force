// TODO INCLUDES
// Includes
#ifndef OPENGLRENDERER_HPP
#define OPENGLRENDERER_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>

// checking for apple, else everyone else
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <SDL2_image/SDL_image.h>
#else
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#endif

// For type definitions etc.
#include "matrix.h"

// For getting the shaders
#include "shader.h"

// OpenGL / glew Headers
// ONLY TESTED ON LINUX
#define GL3_PROTOTYPES 1




#ifdef CreateWindow
#undef CreateWindow
#endif

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// TextureGenerator struct definition
typedef struct TextureGenerator {
	// Width
	//~ GLuint width;
	//~ // Height
	//~ GLuint height;
	// Row
	GLuint rows;
	// Columns
	GLuint columns;
	// File Name
	// NOTE STATIC ALLOCATION
	char textureName[300];
} TextureGenerator;

// BufferAttributes struct definition
typedef struct BufferAttributes {
	GLfloat width;
	GLfloat height;
	GLuint numVertices;
	GLuint textureID;
	GLuint bufferIDStart;
	GLuint bufferIDEnd;
} BufferAttributes;

// Need to think about how to integrate this class with other entity classes
class RenderObject
{
	public:
		// Constructor
		RenderObject(GLfloat initX, GLfloat initY, GLfloat initZ, BufferAttributes initBufferAttributes);
		~RenderObject();
		void ChangeCoordinates(GLfloat newX, GLfloat newY, GLfloat newZ);
		bool FinalFrame();
		void IterateFrame();
		void ForceFrame();
	// Bad taste to make these public
	// Also probably better to have this just be a stuct or something?
	//~ private:
		GLfloat x;
		GLfloat y;
		GLfloat z;
		//~ GLfloat width;
		//~ GLfloat height;
		// I theoretically *could* have a mapping from bufferID to numVertices or something, but I don't know...

		BufferAttributes bufferAttributes;

		//~ std::string textureName;

		//~ int numVertices;
		//~ GLuint textureID;
		GLuint currentBufferID;
		//~ GLuint bufferIDStart;
		//~ GLuint bufferIDEnd;

		// bufferID should be the same as the vaoID
		// GLuint vaoID;
		// Transformation matrix?
		mat4 ctm;
		// Index in renderObjects (in OpenGLRenderer)
		int index;

		// Wait for animations
		int wait;
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
		OpenGLRenderer(SDL_Window* gWindow);
		// Destructor
		//~ ~OpenGLRenderer();
		void Close();
		//~ int CreateWindow();
		// CONTRIVED AT THE MOMENT
		void PopulateTextures();
		void AppendRenderObject(RenderObject *newRenderObject);
		void RemoveRenderObject(int index);
		void TabulaRasa();
		void Display();
		GLuint PopulateDefault2DBuffer(
			//~ BufferAttributes *bufferAttributes,
			GLuint textureID,
			int width,
			int height,
			//~ GLfloat z,
			GLfloat texLeft,
			GLfloat texRight,
			GLfloat texBottom,
			GLfloat texTop
		);
		void PopulateDefault2DBuffers(
			char *fileName,
			//~ int width,
			//~ int height,
			int rows,
			int columns
			//~ BufferAttributes *bufferAttributes
		);
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

		// All of the bufferAttributes
		std::map<std::string, BufferAttributes> allBufferAttributes;
};

GLfloat CanonicalCoordinatesFromPixels(int pixels, int dimension);
#endif
