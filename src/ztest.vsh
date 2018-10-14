#version 150

in vec3 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 ctm;
uniform mat4 projection;
uniform mat4 model_view;

void main()
{
	color = vColor;
	gl_Position = projection * model_view * ctm * vec4(vPosition, 1.0);
}
