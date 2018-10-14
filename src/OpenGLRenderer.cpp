// TODO INCLUDES
#include "helperFunctions.h"
#include "OpenGLRenderer.hpp"

// TODO GLOBALS? PERHAPS I SHOULD MAKE THEM MERELY ATTRBUTES?

// Nedd to think about how to integrate this class with other entity classes
class RenderObject
{
	public:
		// Constructor
		RenderObject(double initX, double initY, double initWidth, double initHeight)
		{
			// I guess I am supposed to use an initializer list
			x = initX;
			y = initY;
			width = initWidth;
			height = initHeight;
		}
	private:
		double x;
		double y;
		double width;
		double height;
}

class OpenGLRenderer
{
	public:
		// Constructor
		OpenGLRenderer()
		{
			// Enable double buffering for OpenGL
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			gWindow = SDL_CreateWindow(
				// Title
				"XXXtreme Peace King: Coronation MEGAMIX",
				// Window position
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				// Width
				SCREEN_WIDTH,
				// Height
				SCREEN_HEIGHT,
				// Note that this means the window is meant for OpenGL
				SDL_WINDOW_OPENGL
			);
			if (gWindow == nullptr)
			{
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				return  false;
			}

			// Enable double buffering for OpenGL
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			// Set the main context for opengl
			mainContext = SDL_GL_CreateContext(gWindow);

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

			// OpenGL texture parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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

			// We theoretically *could* have one static buffer, but we would have to pass it the data for every possible thing to render all at once
			// On one hand, this general idea is interesting because drawing repeats would not duplicate the buffer
			// That is to say, it would draw from the same place in memory to make the repeats
			// How one might store something like this is an interesting problem (it cannot be part of an enemy class!)
			// Would it be reasonable to exhaustively define all possible 3D objects in a file and import them all together?
			// Perhaps it may be best to bind the buffers after iterating over all current 3D objects
			//~ GLuint buffer;
			//~ glGenBuffers(1, &buffer);
			//~ glBindBuffer(GL_ARRAY_BUFFER, buffer);
			//~ glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
			//~ glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			//~ glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
			glEnableVertexAttribArray(vTexCoord);
			glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

			// This is for straight colors
			//~ GLuint vColor = glGetAttribLocation(program, "vColor");
			//~ glEnableVertexAttribArray(vColor);
			//~ glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

			// Not sure about global transformation matrix...
			//~ ctm_location = glGetUniformLocation(program, "ctm");

			// GLOBAL: APPLIED TO ALL
			projection_matrix_location = glGetUniformLocation(program, "projection");
			model_view_matrix_location = glGetUniformLocation(program, "model_view");

			glEnable(GL_DEPTH_TEST);
			// If you want black
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glDepthRange(1,0);
		}
		// Destructor
		~OpenGLRenderer()
		{
			// Delete our OpengL context
			SDL_GL_DeleteContext(mainContext);

			SDL_DestroyWindow(gWindow);
			gWindow = nullptr;
		}
		void PopulateTextures(char[] textureName)
		{
			for (int textureID=0; textureID < TEXTURES.length; textureID++)
			{
				char[] textureName = TEXTURES[i];
				SDL_Surface* surface = IMG_Load(textureName);

				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);

				int Mode = GL_RGB;

				if(surface->format->BytesPerPixel == 4) {
					Mode = GL_RGBA;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
		void AppendRenderObject(RenderObject newRenderObject)
		{
			RenderObjects.push_back(newRenderObject);
		}
		void Display()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// I can reverse the order to change the effect from rotation in place to rotation about axis!
			//~ mat4_multiplication(&tr, &rot, &ctm);

			// The current transformation matrix depends on the current object
			//~ glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

			glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, (GLfloat *) &projection);
			glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &frame);

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_LINE);
			// Triangles are more efficient, but quads easier to work with
			//~ glDrawArrays(GL_TRIANGLES, 0, num_vertices);
			//~ glDrawArrays(GL_QUADS, 0, num_vertices);

			// Render every "renderObject"
			for (auto renderObject: renderObjects) {
				glBindTexture(GL_TEXTURE_2D, renderObject.textureID);

				// Uncertain as to whether using glBegin this way is inappropriate
				// Basically, it is supposed to make a quad with the texture
				// (Assuming everything worked)
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex3f(renderObject.x, renderObject.y, 0);
					glTexCoord2f(1, 0); glVertex3f(renderObject.x + renderObject.width, renderObject.y, 0);
					glTexCoord2f(1, 1); glVertex3f(renderObject.x + renderObject.width, renderObject.y + renderObject.height, 0);
					glTexCoord2f(0, 1); glVertex3f(renderObject.x, renderObject.y + renderObject.height, 0);
				glEnd();
			}

			glEnable(GL_CULL_FACE);

			SDL_GL_SwapWindow(gWindow);
		}
	private:
		// Number of vertices (needed to render)
		int number_of_vertices;
		// The camera
		mat4 frame;
		// The projection (frustum, etc)
		mat4 projection;
		// An array of objects to render
		std::vector<RenderObject>;
		// TODO
		// The window to display to
		SDL_Window* gWindow;
		// The opengl context handle
		SDL_GLContext mainContext;
};
