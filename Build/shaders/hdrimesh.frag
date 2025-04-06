#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;


uniform vec3 cameraPos;
uniform sampler2D skybox;

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = texture(skybox, TexCoords);
}