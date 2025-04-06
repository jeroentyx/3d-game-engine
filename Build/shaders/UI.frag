#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D myTextureSampler;
//uniform sampler2D u_Texture[32];

void main()
{
    color = texture(myTextureSampler, v_TexCoord);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
    
    // This is new
    //color = texture(myTextureSampler[int(v_TexIndex)], v_TexCoord) * v_Color;
}