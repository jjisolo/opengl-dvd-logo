#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

uniform float textureMoveOffsetX;
uniform float textureMoveOffsetY;

out vec3 textureColor;
out vec2 textureCoordinates;

void main()
{
	gl_Position        = vec4(aPos.x+textureMoveOffsetX, aPos.y+textureMoveOffsetY, aPos.z, 1.0);

	// Ret
	textureColor       = aColor;
	textureCoordinates = aTextCoord;
}