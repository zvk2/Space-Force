#version 120

varying vec2 texCoord;

uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, texCoord);
    
    // Manipulating this alpha cutoff has amusing side effects
    if (gl_FragColor.a <= 0.001)
    {
        discard;
    }
}
