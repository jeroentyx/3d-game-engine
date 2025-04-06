#pragma once

#include "Texture.h"
#include <glm/glm.hpp>

//
//Wanted to Wrap OpenGLTexture to this, but choose otherwise
// will wrapped it inside Constructor
class OpenGLGuiTexture
{
public:


    OpenGLGuiTexture(
        const glm::vec2& m_Scale,
        const glm::vec2& m_Position,
        const std::string& path
    );

    glm::vec2 m_Scale, m_Position;
    std::shared_ptr<Texture2D> m_UIElements;

    //Do i need this?
    void CreateVAOLayout() {};

    //Do binding vertex Array, enable Attribute, do it like FBO like that
    //Camera how? LocalToWorld MVP matrix, Proj and mdl matrix
    void Render() {};

    //can get from m_UIElements <Texture2D>
    //std::string m_Path;
    //GLuint m_Width, m_Height; //This will be scissored?
    //GLuint m_RendererID;


};