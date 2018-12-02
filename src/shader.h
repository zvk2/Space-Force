#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
// #include "INC_SDL.h"
#include <GL/glew.h>

class Shader
{
    public:
    	Shader(const std::string);
        ~Shader();
        GLuint getProgram();

    private:
    	const std::string readShaderSource(const std::string);
    	GLuint createShader(const std::string, GLenum);
        std::string getShaderType(GLuint);
        void checkCompileError(GLuint);
        void checkLinkError();

    	GLuint program;
    	GLuint vShader;
    	GLuint fShader;
};

#endif
