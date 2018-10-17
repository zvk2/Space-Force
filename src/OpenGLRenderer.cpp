// clang++ -o 3dtest.exe -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lglut -lGL -lGLU -lm src/OpenGLRenderer.cpp src/initShader.c src/helperFunctions.c -I/usr/include/SDL2

// TODO INCLUDES
#include "OpenGLRenderer.hpp"

// TODO GLOBALS? PERHAPS I SHOULD MAKE THEM MERELY ATTRBUTES?

// Need to think about how to integrate this class with other entity classes
// Constructor
RenderObject::RenderObject(double initX, double initY, double initWidth, double initHeight, GLuint initTextureID)
{
	// I guess I am supposed to use an initializer list, but hey
	x = initX;
	y = initY;
	width = initWidth;
	height = initHeight;
	textureID = initTextureID;
}
// Destructor
// EMPTY FOR NOW
RenderObject::~RenderObject() {};

// Constructor
OpenGLRenderer::OpenGLRenderer()
{
	// Initialzie renderObjects to an empty vector (can be dynamically populated)
	renderObjects = std::vector<RenderObject>();

	// Create the window and continue if possible
	if (CreateWindow())
	{
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		//~ SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

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

		// NEED TO SWITCH TO DYLAN'S SHADER
		Shader shader("ztest");
		program = shader.getProgram();

		// Vertex array object, basically what to send to be rendered
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// If you want black
		glClearColor(0.0, 0.0, 0.0, 1.0);

		// Enable blending
		// (For dealing with transparency, don't want blasted blocks all over!)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Get the textures
		PopulateTextures();
	}
}
// Destructor
// Not really the right way of freeing, methinks
//~ OpenGLRenderer::~OpenGLRenderer()
//~ {
	//~ // Delete our OpengL context
	//~ SDL_GL_DeleteContext(mainContext);

	//~ SDL_DestroyWindow(gWindow);
	//~ gWindow = nullptr;

	//~ // Delete vector
	//~ // BAD
	//~ renderObjects.clear();
//~ }
void OpenGLRenderer::Close()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	// Delete vector
	// BAD
	renderObjects.clear();
}
int OpenGLRenderer::CreateWindow()
{
	// Enable double buffering for OpenGL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Actually create the window
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
	return true;
}
// CONTRIVED AT THE MOMENT
void OpenGLRenderer::PopulateTextures()
{
	// CONTRIVED
	// NOTE THAT A LIST OF TEXTURES LIKE THIS COULD GET UGLY RATHER QUICKLY...
	std::vector<std::string> textureNames = {
		"resources/test.png",
		"resources/test2.png",
	};

	// VERY CONTRIVED
	textureIDs = std::vector<GLuint>(textureNames.size());
	bufferIDs = std::vector<GLuint>(textureNames.size());

	// CONTRIVED LOOP
	for (GLuint textureID=0; textureID < textureNames.size(); textureID++)
	{
		// Add to textureIDs (needed to bind texture when rendering)
		textureIDs[textureID] = textureID;
		bufferIDs[textureID] = textureID;

		// Get a cstyle string for loading the image
		char textureName[textureNames[textureID].size() + 1];
		strcpy(textureName, textureNames[textureID].c_str());

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
		glGenTextures(1, &textureIDs[textureID]);
		// Indicate where this new texture will be bound
		glBindTexture(GL_TEXTURE_2D, textureIDs[textureID]);

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
		vec2 texCoord[6] = {
			{0.0, 0.0},
			{0.0, 1.0},
			{1.0, 1.0},
			{0.0, 0.0},
			{1.0, 1.0},
			{1.0, 0.0},
		};

		// Describes how we will be sending data out to be rendered
		glGenBuffers(1, &bufferIDs[textureID]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[textureID]);
		// Full buffer
		glBufferData(GL_ARRAY_BUFFER, verticesSize + sizeof(texCoord), NULL, GL_STATIC_DRAW);
		// Vertices
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, vertices);
		// Texture stuff
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, sizeof(texCoord), texCoord);

		// Info for position (vec4 at the moment)
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Info for the texture (vec2 at the moment)
		GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoord);
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) verticesSize);

		// FREE THE SURFACE
		SDL_FreeSurface(surface);
	}
}
// This just appends the render object
void OpenGLRenderer::AppendRenderObject(RenderObject newRenderObject)
{
	renderObjects.push_back(newRenderObject);
}

void OpenGLRenderer::Display()
{
	// Clear initially
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Iterate over every object to render
	for (auto currentObject: renderObjects)
	{
		// Contrived for one rectangle
		int numVertices = 6;
		// Which texture to use
		// (Somewhat crude)
		glBindTexture(GL_TEXTURE_2D, textureIDs[currentObject.textureID]);
		// Also pick the right buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[currentObject.textureID]);

		// Didn't add a transform matrix yet
		//~ ctm_location = glGetUniformLocation(program, "ctm");

		// Enable the texture
		// TECHNICALLY CAN BE USED TO SWAP TEXTURES IN AN ARRAY
		// HOWEVER, YOU CAN ALSO JUST CHANGE WHAT TEXTURE IS BOUND
		glUniform1i(glGetUniformLocation(program, "texture"), 0);

		// Draw vertices in the buffer
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

	//~ // Get rid of faces in the wrong direction
	//~ glEnable(GL_CULL_FACE);
	//~ // Actually test for depth
	//~ glEnable(GL_DEPTH_TEST);
	// Clear to black
	glClearColor(0, 0, 0, 1.0);
	// Set range for depth
	glDepthRange(1, 0);

	// Actually show everything when done
	SDL_GL_SwapWindow(gWindow);
}

// This main function exists to test OpenGLRenderer
int main(int argc, char* argv[])
{
	// Spawn an instance of OpenGLRenderer
	OpenGLRenderer openGL = OpenGLRenderer();

	// Rough sketch of a RenderObject?
	RenderObject test = RenderObject(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0
	);

	RenderObject test2 = RenderObject(
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1
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
