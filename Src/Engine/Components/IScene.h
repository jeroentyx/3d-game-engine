#pragma once

#include "glm/glm.hpp"

//
//  A Base class of the Actually RenderingSystem
//
class IScene {
protected:
    glm::mat4 model, view, projection;

public:
    int width;
    int height;

    IScene() : m_animate(true), width(800), height(600), model(glm::mat4(1.0f)), view(glm::mat4(1.0)), projection(glm::mat4(1.0)) {}
    virtual ~IScene() {}


    void setDimension(int w, int h) {
        width = w;
        height = h;
    }

    //
    //  Load textures, Initialize shaders, etc 
    //
    virtual void initScene() = 0;

    //
    //  This is called prior to every frame, Use this to
    //  update your animation
    //
    virtual void update(float t) = 0;

    //
    //  Draw your scene
    //
    virtual void render() = 0;
    virtual void resize(int, int) = 0;

    void animate(bool value) { m_animate = value; }
    bool isAnimating() { return m_animate; }
protected:
    bool m_animate;
};