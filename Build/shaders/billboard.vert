#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 UV;

uniform bool FixedSize;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard (world units)

void main()
{
    if (FixedSize)
    {
        //vec3 particleCenter_wordspace = BillboardPos;
        //vec3 vertexPosition_worldspace = particleCenter_wordspace;
        //
        //gl_Position = u_ViewProjection * u_Transform * vec4(vertexPosition_worldspace, 1.0f);
        //gl_Position /= gl_Position.w;
        //
        //gl_Position.xy += aPos.xy * vec2(1.0, 1.0);
    }
    else
    {
        vec3 particleCenter_wordspace = BillboardPos;
    
        vec3 vertexPosition_worldspace = 
            particleCenter_wordspace
            + CameraRight_worldspace * aPos.x * BillboardSize.x
            + CameraUp_worldspace * aPos.y * BillboardSize.y;
        
        // Output position of the vertex
        gl_Position = u_ViewProjection * u_Transform * vec4(vertexPosition_worldspace, 1.0f);
    }
    
    
    UV = aPos.xy + vec2(0.5, 0.5);
    
    //gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0f);
}