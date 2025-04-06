#version 330 core

layout (location = 0) in vec3 squareVertices;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform bool FixedSize;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard (world units)

void main()
{
    //vec3 particleCenter_wordspace = BillboardPos;
    //
    //vec3 vertexPosition_worldspace = 
    //    particleCenter_wordspace
    //    + CameraRight_worldspace * squareVertices.x * BillboardSize.x
    //    + CameraUp_worldspace * squareVertices.y * BillboardSize.y;
    //
    //// Output position of the vertex
    //gl_Position = u_ViewProjection * u_Transform * vec4(vertexPosition_worldspace, 1.0f);
    //
    //
    //UV = squareVertices.xy + vec2(0.5, 0.5);
    
    
    
    gl_Position = u_ViewProjection * u_Transform * vec4(squareVertices, 1.0f);
    v_TexCoord = texCoord;
}