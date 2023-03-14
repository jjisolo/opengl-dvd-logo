#version 400 core

out vec4 FragColor;

in vec3 textureColor;
in vec2 textureCoordinates;

uniform sampler2D dvdTexture;

void main()
{
	FragColor = texture(dvdTexture, textureCoordinates);
}
