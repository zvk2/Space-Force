#version 130

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;

void main()
{
	fColor = texture2D(texture, texCoord);
	// Manipulating this alpha cutoff has amusing side effects
	if (fColor.a <= 0.001)
	{
		discard;
	}
}
