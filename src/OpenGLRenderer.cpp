// clang++ -o 3dtest.exe -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lglut -lGL -lGLU -lm src/OpenGLRenderer.cpp src/matrix.c src/shader.cpp -I/usr/include/SDL2

// TODO INCLUDES
#include "OpenGLRenderer.hpp"

// TODO GLOBALS? PERHAPS I SHOULD MAKE THEM MERELY ATTRBUTES?

// Get coords from pixels
GLfloat CanonicalCoordinatesFromPixels(int pixels, int dimension)
{
	return (pixels - (dimension / 2.0)) / (dimension / 2.0);
}

// Need to think about how to integrate this class with other entity classes
// Constructor
RenderObject::RenderObject(GLfloat initX, GLfloat initY, GLfloat initZ, BufferAttributes initBufferAttributes)
//~ RenderObject::RenderObject(GLfloat initX, GLfloat initY, GLfloat initWidth, GLfloat initHeight, int vertices, GLuint initTextureID, GLuint initBufferID)
{
	// I guess I am supposed to use an initializer list, but hey
	bufferAttributes = initBufferAttributes;

	currentBufferID = bufferAttributes.bufferIDStart;

	//~ ctm = translation_matrix(x, y, 0);

	ChangeCoordinates(initX, initY, initZ);
}
// Destructor
// EMPTY FOR NOW
RenderObject::~RenderObject() {};

// Change coordinates
void RenderObject::ChangeCoordinates(GLfloat newX, GLfloat newY, GLfloat newZ)
{
	x = newX;
	y = newY;
	GLfloat trX = CanonicalCoordinatesFromPixels(newX, SCREEN_WIDTH) + 1;
	GLfloat trY = -CanonicalCoordinatesFromPixels(newY, SCREEN_HEIGHT) - 1;
	z = newZ;
	ctm = translation_matrix(trX, trY, z);
}

// Need to think about how to integrate this class with other entity classes
// Constructor
//~ RenderObject::RenderObject() {};
//~ // Destructor
//~ // EMPTY FOR NOW
//~ RenderObject::~RenderObject() {};

// Constructor
OpenGLRenderer::OpenGLRenderer(SDL_Window* window, std::map<std::string, BufferAttributes> *initAllBufferAttributes)
{
	// Make sure buffer matched up
	allBufferAttributes = initAllBufferAttributes;

	// Initialzie renderObjects to an empty vector (can be dynamically populated)
	renderObjects = std::vector<RenderObject*>();

	// Assign window
	gWindow = window;

	// Init statically allocated vectors
	// VERY CONTRIVED
	textureIDs = std::vector<GLuint>();
	bufferIDs = std::vector<GLuint>();
	vaoIDs = std::vector<GLuint>();

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
	// TODO REMOVE
	//~ SDL_GL_SetSwapInterval(1);

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

	// If you want black
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Enable blending
	// (For dealing with transparency, don't want blasted blocks all over!)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// Clip outside
	glEnable(GL_CULL_FACE);

	// Transformation matrix
	ctmLocation = glGetUniformLocation(program, "ctm");

	// Get the textures
	PopulateTextures();
}
void OpenGLRenderer::PopulateTextures()
{
	// Crude!
	TextureGenerator textureGenerators[] =
	{
		{1280, 720, 1, 1, "resources/test.png"},
		{1280, 720, 1, 1, "resources/test2.png"},
		{1280, 720, 1, 1, "resources/imgs/space_2_background.png"},
	};
	// Iterate over every texture to generate
	for (auto currentGenerator: textureGenerators)
	{
		PopulateDefault2DBuffers(
			// OpenGL instance
			this,
			// File Name
			currentGenerator.textureName,
			// Width
			currentGenerator.width,
			// Height
			currentGenerator.height,
			// Row
			currentGenerator.rows,
			// Columns
			currentGenerator.columns
		);
	}
}
void OpenGLRenderer::Close()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	// Delete vector
	// BAD
	for (int i=0; i<renderObjects.size(); i++)
	{
		delete renderObjects[i];
	}

	renderObjects.clear();
}
// This just appends the render object
void OpenGLRenderer::AppendRenderObject(RenderObject *newRenderObject)
{
	newRenderObject->index = renderObjects.size();
	renderObjects.push_back(newRenderObject);
}
// TODO
void OpenGLRenderer::RemoveRenderObject(int index)
{
	// Decrement indices
	for (int i=index; i<renderObjects.size(); i++)
	{
		renderObjects[i]->index -= 1;
	}
	// Uhh... it might be a bad idea to delete renderObjects here?
	// Problem is other things might call it before then
	delete renderObjects[index];
	// Actually remove the object
	renderObjects.erase(renderObjects.begin() + index);
}
void OpenGLRenderer::Display()
{
	// Clear initially
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Iterate over every object to render
	for (auto currentObject: renderObjects)
	{
		// Note dereference, world is a fuf
		BufferAttributes bufferAttributes = currentObject->bufferAttributes;

		//~ std::cout << bufferAttributes.textureID << std::endl;
		// AREN'T SUPPOSED TO HAVE A BUNCH OF VAOS BUT WHATEVER
		glBindVertexArray(vaoIDs[currentObject->currentBufferID]);
		// Which texture to use
		// (Somewhat crude)
		glBindTexture(GL_TEXTURE_2D, textureIDs[bufferAttributes.textureID]);
		// Also pick the right buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[currentObject->currentBufferID]);

		// Transformation matrix
		//~ std::cout << "x: " << currentObject->ctm.w.x << " Y: " << currentObject->ctm.w.y << std::endl;
		glUniformMatrix4fv(ctmLocation, 1, GL_FALSE, (GLfloat *) &currentObject->ctm);

		// Enable the texture
		// TECHNICALLY CAN BE USED TO SWAP TEXTURES IN AN ARRAY
		// HOWEVER, YOU CAN ALSO JUST CHANGE WHAT TEXTURE IS BOUND
		glUniform1i(glGetUniformLocation(program, "texture"), 0);

		// Draw vertices in the buffer
		glDrawArrays(GL_TRIANGLES, 0, bufferAttributes.numVertices);
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

GLuint PopulateDefault2DBuffer(
	OpenGLRenderer* openGL,
	//~ char *textureName,
	GLuint currentTexture,
	int width,
	int height,
	//~ GLfloat z,
	GLfloat texLeft,
	GLfloat texRight,
	GLfloat texBottom,
	GLfloat texTop
)
{
	//~ int currentTexture = openGL->textureIDs.size();
	int currentBuffer = openGL->bufferIDs.size();
	int currentVao = openGL->vaoIDs.size();
	//~ openGL->textureIDs.push_back(currentTexture);
	openGL->bufferIDs.push_back(currentBuffer);
	openGL->vaoIDs.push_back(currentVao);

	glGenVertexArrays(1, &openGL->vaoIDs[currentVao]);
	glBindVertexArray(openGL->vaoIDs[currentVao]);

	// Texture parameters
	// Basically, repeat if you need to and linear interpolation for texel -> pixel
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Contrived for one rectangle
	int numVertices = 6;
	int verticesSize = numVertices * sizeof(vec4);

	GLfloat bottom = -CanonicalCoordinatesFromPixels(height, SCREEN_HEIGHT);
	GLfloat right = CanonicalCoordinatesFromPixels(width, SCREEN_WIDTH);
	GLfloat top = 1;
	GLfloat left = -1;

	// z is 0 due to a contrivance
	GLfloat z = 0;

	// One rectangle
	// WE SHOULD PROBABLY HAVE A USER-DEFINED z
	vec4 vertices[6] = {
		{left,  top,  z, 1.0},	// front top left
		{left, bottom,  z, 1.0},	// front bottom left
		{ right, bottom,  z, 1.0},	// front bottom right
		{left,  top,  z, 1.0},	// front top left
		{ right, bottom,  z, 1.0},	// front bottom right
		{ right,  top,  z, 1.0},	// front top right
	};

	//~ std::cout << "Left: " << left << " Top: " << top << std::endl;
	//~ std::cout << "Right: " << right << " Bottom: " << bottom << std::endl;

	// **PLEASE NOTE THIS IS UPSIDE DOWN**
	// Why? I think (though I am not sure) that the surface pixels from SDL are upside down
	// That is, (0, 0) is top left from SDL's perspective, HOWEVER (0, 1) is top left from OpenGL's perspective
	vec2 texCoord[6] = {
		{texLeft, texTop},
		{texLeft, texBottom},
		{texRight, texBottom},
		{texLeft, texTop},
		{texRight, texBottom},
		{texRight, texTop},
	};

	// Describes how we will be sending data out to be rendered
	glGenBuffers(1, &openGL->bufferIDs[currentBuffer]);
	glBindBuffer(GL_ARRAY_BUFFER, openGL->bufferIDs[currentBuffer]);
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

	return currentBuffer;
}
void PopulateDefault2DBuffers(
	OpenGLRenderer* openGL,
	char *textureName,
	int width,
	int height,
	int rows,
	int columns
	//~ BufferAttributes *bufferAttributes
)
{
	int currentRow = 0;
	int currentColumn = 0;

	GLfloat currentRowCoordinate = 0;
	GLfloat rowOffset = 1 / rows;
	GLfloat currentColumnCoordinate = 0;
	GLfloat columnOffset = 1 / columns;

	// Get texture
	GLuint currentTexture = openGL->textureIDs.size();
	openGL->textureIDs.push_back(currentTexture);

	// Get a cstyle string for loading the image
	//~ char textureName[] = fileName;

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
	glGenTextures(1, &openGL->textureIDs[currentTexture]);
	// Indicate where this new texture will be bound
	glBindTexture(GL_TEXTURE_2D, openGL->textureIDs[currentTexture]);

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

	GLuint firstBuffer = openGL->bufferIDs.size();
	GLuint bufferOffset = 0;
	while (currentRow < rows)
	{
		while (currentColumn < columns)
		{
			GLuint currentBuffer = PopulateDefault2DBuffer(
				openGL,
				currentTexture,
				width,
				height,
				//~ z,
				currentColumnCoordinate,
				currentColumnCoordinate + columnOffset,
				currentRowCoordinate + rowOffset,
				currentRowCoordinate
			);

			currentColumnCoordinate += columnOffset;
			currentColumn++;

			bufferOffset++;
		}

		currentRowCoordinate += rowOffset;
		currentRow++;
	}

	// Note dereference, world is a fuf
	std::map<std::string, BufferAttributes> *currentBufferAttributes = openGL->allBufferAttributes;
	(*currentBufferAttributes)[textureName] = {
		// Width
		(GLfloat)width,
		// Height
		(GLfloat)height,
		// verts
		// contrived for now to 2d (3d will be individually defined, methinks)
		6,
		// texture
		currentTexture,
		// buffer start
		firstBuffer,
		// buffer end (changed at the conclusion)
		firstBuffer + bufferOffset
	};

	// FREE THE SURFACE
	SDL_FreeSurface(surface);
}
