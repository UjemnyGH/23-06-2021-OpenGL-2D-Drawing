#version 430 core

uniform mat4 projViModMat;

#define POSITION 0

layout(location = POSITION)in vec4 inPos;

void main()
{
    gl_Position = projViModMat * inPos;
}