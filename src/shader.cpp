#include "shader.h"

Shader::Shader(const std::string name)
{
	program = glCreateProgram();

	const std::string vertexSource = readShaderSource(name + ".vsh");
	const std::string fragSource = readShaderSource(name + ".fsh");

	vShader = createShader(vertexSource, GL_VERTEX_SHADER);
	fShader = createShader(fragSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glBindFragDataLocation(program, 0, "outColor"); // unnecessary at 0, but used when fShader writes to multiple buffers.
                                                    // will figure this out more
	glLinkProgram(program);
    checkLinkError();

    glValidateProgram(program);
    checkLinkError();

    glUseProgram(program);
}

Shader::~Shader()
{
    glDetachShader(program, vShader);
    glDeleteShader(vShader);
    glDetachShader(program, fShader);
    glDeleteShader(fShader);
 
    glDeleteProgram(program);
}

const std::string Shader::readShaderSource(const std::string name)
{
	std::ifstream file("src/" + name);
	std::string text;
	std::string line;

	if(!file.is_open())
	{
		std::cerr << "vshader.glsl failed to open." << std::endl;
		exit(1);
	}

	while(file.good())
	{
		getline(file, line);
		text.append(line + "\n");	
	}

	file.close();
	return text;
}

GLuint Shader::createShader(const std::string source, GLenum type)
{
	GLuint shader = glCreateShader(type);

	const char* cSource = source.c_str();

	glShaderSource(shader, 1, &cSource, NULL);
	glCompileShader(shader);

    checkCompileError(shader);

	return shader;
}

void Shader::checkCompileError(GLuint _shader)
{
    GLint status;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(_shader, sizeof(buffer), NULL, buffer);
        std::cerr << status << " Shader compilation error: " << buffer << std::endl;
    }
}

void Shader::checkLinkError()
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status != GL_TRUE)
    {
        char buffer[512];
        glGetProgramInfoLog(program, sizeof(buffer), NULL, buffer);
        std::cerr << status << " Shader link error: " << buffer << std::endl;
    }
}

GLuint Shader::getProgram()
{
	return program;
}
