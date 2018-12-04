#version 130

in vec4 vPosition;
in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 ctm;
uniform bool flipped;

void main()
{
	texCoord = vTexCoord;

	if (flipped)
	{
		texCoord.x *= -1;
	}

	gl_Position = ctm * vPosition;
	// gl_Position = vPosition;
}
