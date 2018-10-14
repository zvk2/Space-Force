// TODO INCLUDES

// Need to think about how to integrate this class with other entity classes
class RenderObject
{
	public:
		// Constructor
		RenderObject(double initX, double initY, double initWidth, double initHeight);
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
		OpenGLRenderer();
		// Destructor
		~OpenGLRenderer();
		void PopulateTextures;
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
		std::vector<RenderObject>;
		// TODO
		// The window to display to
		SDL_Window* gWindow;
		// The opengl context handle
		SDL_GLContext mainContext;
};
