#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

//uniform bool FixedSize;
//uniform vec3 CameraRight_worldspace;
//uniform vec3 CameraUp_worldspace;
//uniform vec3 BillboardPos; // Position of the center of the billboard
//uniform vec2 BillboardSize; // Size of the billboard (world units)

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec4 v_Color
in vec2 v_TexCoord;
in float v_TexIndex;

//uniform sampler2D myTextureSampler;
uniform sampler2D u_Texture[32];

void main()
{
    //color = texture(myTextureSampler, v_TexCoord);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
    
    // This is new
    color = texture(u_Texture[int(v_TexIndex)], v_TexCoord) * v_Color;
}