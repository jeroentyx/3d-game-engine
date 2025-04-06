#type vertex
#version 440 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
}


#type fragment
#version 440 core

layout (location = 0) out vec4 finalcolor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
//uniform vec4 bgColor;
//uniform vec4 fgColor;

float median(float r, float g, float b) 
{
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange()
{
    vec2 unitRange = vec2(2.0)/vec2(textureSize(u_Texture, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(v_TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() 
{
    vec3 msd = texture(u_Texture, v_TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    
    finalcolor = vec4(1.0, 1.0, 1.0, opacity);
    
    //finalcolor = texture(u_Texture, v_TexCoord);
}