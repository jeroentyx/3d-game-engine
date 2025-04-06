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

struct Smoke_Component : public IComponent
{
public:
    GLSLProgram prog, flatProg; //One for particle system, another for Grid
    glm::mat4 model, view, projection;
    glm::vec3 emitterPos, emitterDir;
    int width;
    int height;

    //Parameters for ParticleSystem
    GLuint posBuf[2], velBuf[2], age[2];
    GLuint particleArray[2];
    GLuint feedback[2];
    GLuint drawBuf;

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
    Smoke_Component();
    ~Smoke_Component();

    void initScene();
    void Init() override;
    void update(float t);
    void PassInUniforms(glm::mat4 _view, glm::mat4 _proj, glm::mat4 _model);
    void render();
    void resize(int, int);
    void setDimension(int w, int h);
    void SetFilePath(std::string& path);
    property_vtable()
};

property_begin(Smoke_Component)
{
    property_var(nParticles)
        , property_var(particleLifeTime)
        , property_var(emitterPos)
        , property_var(emitterDir)
        , property_var(filePath)
}property_vend_h(Smoke_Component)