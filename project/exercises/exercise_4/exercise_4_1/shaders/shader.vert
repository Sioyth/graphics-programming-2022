#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 velocity;
layout (location = 2) in float timeOfBirth;

void main()
{
	gl_Position = vec4(pos.xy, 0.0f, 0.0f);
}