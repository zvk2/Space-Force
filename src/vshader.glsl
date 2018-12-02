#version 120

attribute vec4 vPosition; // in
attribute vec4 vColor;    // in

varying vec4 color;       // out
varying vec3 texCoord;    // out

uniform mat4 look;

void main()
{
	// color and vColor were once a hacky place to store normals. i think they
	// are now entirely useless, but i left 'em in just incase/out of laziness
	color = vColor;
	gl_Position = look*vPosition;
	texCoord = vec3(vPosition.x, vPosition.y, vPosition.z);
}
