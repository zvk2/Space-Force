#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#endif

class Shader
{
    public:
    	Shader(const std::string);
        ~Shader();
        GLuint getProgram();

    private:
    	const std::string readShaderSource(const std::string);
    	GLuint createShader(const std::string, GLenum);
        void checkCompileError(GLuint shader);
        void checkLinkError();

    	GLuint program;
    	GLuint vShader;
    	GLuint fShader;
};

#endif
