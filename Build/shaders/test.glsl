#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    v_TexCoord = aTex;
}


#type fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
}