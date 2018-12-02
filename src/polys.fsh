#version 120

varying vec4 color;
varying vec3 texCoord;

uniform samplerCube cubeMap;

void main()
{
	vec4 texel = textureCube(cubeMap, texCoord);
	gl_FragColor = color;
}
