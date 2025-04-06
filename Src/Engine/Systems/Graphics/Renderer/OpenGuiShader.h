#pragma once

#include <string>
#include <glm/glm.hpp>

class OpenGLGuiShader
{
public:
    //change this to vertex,fragment path instead
    std::string VERTEX_FILE = "Buttons/PlayButton.png";
    std::string FRAGMENT_FILE = "Buttons/PlayButton.png";

    uint32_t TransformationMatrix_Loc;

    OpenGLGuiShader():
        TransformationMatrix_Loc(0)
    {

    }

    //LocalToWorld is MVP
    void SendMVPMatrix(const glm::mat4& LocalToWorldMatrix)
    {

    }

};