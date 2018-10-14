#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
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
        void checkCompileError(GLuint shader);
        void checkLinkError();

    	GLuint program;
    	GLuint vShader;
    	GLuint fShader;
};

#endif
