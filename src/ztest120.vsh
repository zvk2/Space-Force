#version 120

attribute vec4 vPosition;
attribute vec2 vTexCoord;

varying vec2 texCoord;

uniform mat4 ctm;

void main()
{
    texCoord = vTexCoord;
    gl_Position = ctm * vPosition;
}
