#version 330

out vec4 color;
in vec4 vColor;
in vec2 TexCoord;

uniform sampler2D theTexture;

void main()
{
    color = texture(theTexture, TexCoord);
}