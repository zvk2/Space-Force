#version 130

in vec4 vPosition;
in vec2 vTexCoord;
out vec2 texCoord;

// uniform mat4 ctm;

void main()
{
	texCoord = vTexCoord;
	//~ gl_Position = ctm * vPosition;
	gl_Position = vPosition;
}
