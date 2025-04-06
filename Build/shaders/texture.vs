#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out float out_opacity;

uniform mat4 LocalToWorld;
uniform float Opacity;
uniform vec3 in_Color;
uniform float Depth;

void main()
{
	gl_Position = LocalToWorld * vec4(aPos, 1.0);
	gl_Position.z = Depth;
	ourColor = in_Color;
	out_opacity = Opacity;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}