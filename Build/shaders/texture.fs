#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in float out_opacity;

// texture sampler
uniform sampler2D texture1;
uniform bool gamma;

void main()
{
	vec3 blackColor = ourColor;
	if(gamma)
	{
		 blackColor = pow(blackColor, vec3(1.0/2.2));
	}
	FragColor = texture(texture1, TexCoord) * vec4(blackColor,out_opacity);

}