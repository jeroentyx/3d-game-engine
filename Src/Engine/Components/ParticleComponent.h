#pragma once
#include "Src/Engine/Core/Interfaces/IComponent.h"
#include "Src\pch.h"
#include "Particlehelper.h"
#include "Particle.h"
#include "Src\Engine\Systems\Graphics\Compiler\TextureLoader.h"
#include "Src/Engine/Core/Interfaces/IComponent.h"
#include "IScene.h"

#include "glslprogram.h"
#include <GL/glew.h>
#include "grid.h"

#include <glm/glm.hpp>

struct Particle_Component : public IComponent
{
public:
    GLSLProgram prog, flatProg; //One for particle system, another for Grid
    glm::mat4 model, view, projection;
    int width;
    int height;

//Parameters for ParticleSystem
    GLuint posBuf[2], velBuf[2], age[2];
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;
    
    Grid grid;

    //to serialize nparticles, angle, particle size and lifetime
    int nParticles;
    
    float angle;
    float time, deltaT;
    float particleSize, particleLifeTime;
    GLuint textureID;
    std::string filePath;
    void SetMatrices(GLSLProgram&);
    void compileAndLinkShader();
    void initBuffers();

public:
    Particle_Component();
    ~Particle_Component();

    void Init() override;
    void initScene();
    void update(float t);
    void PassInUniforms(glm::mat4 _view, glm::mat4 _proj,glm::mat4 _model);
    void render();
    void resize(int, int);
    void setDimension(int w, int h);
    void SetFilePath(std::string& path);

    property_vtable()
};

property_begin(Particle_Component)
{
    property_var(angle)
  , property_var(particleSize)
  , property_var(particleLifeTime)
  , property_var(filePath)
}property_vend_h(Particle_Component)
