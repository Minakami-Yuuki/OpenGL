#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 u_Color;

void main()
{
   gl_Position = position;
   u_Color = color;
};


#shader fragment
#version 330 core

// in vec4 u_Color;
out vec4 color;

uniform vec4 u_Color;

void main()
{
   color = u_Color;
};