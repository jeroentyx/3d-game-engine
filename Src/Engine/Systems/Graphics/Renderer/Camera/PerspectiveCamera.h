#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Src\pch.h"


#include "Tools\FixedTimer.h"

#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"

#include "Src\Profiler\FrameRate.h"
#include "Src\Engine\Components\Transform\TransformComponent.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"
#include "Src\ImGui\Context.h"

class PerspectiveCamera
{
public:
    float cameraSpeed = 0.05f;

    bool cameraMode = true;
    bool firstMouse = true;
    float yaw = -89.0f;
    float pitch = 0.0f;
    float lastX = 800.0f / 2.0;
    float lastY = 600.0 / 2.0;
    float fov = 20.0f;

    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0, 0.0f);
    glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::mat4 m_PlayerViewMatrix;
    glm::mat4 m_PlayerProjectMatrix;


    int32_t scr_width;
    int32_t scr_height;
    int32_t gamescreen_posX;
    int32_t gamescreen_posY;

    
public:
    bool hovered{ false };

    PerspectiveCamera();
    void Init(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.6f),
        glm::vec3 up = glm::vec3(0, 1, 0),
        float Yaw = -90.0f,
        float pitch = 0.0f);
    PerspectiveCamera(const glm::vec3& startingPosition);

    bool m_active = true;

    // Getters
    const glm::vec3& GetCenter();
    const glm::vec3& GetEye();
    const glm::vec3& GetUp();

    // Setters
    void SetCenter(float x, float y, float z);
    void SetCenter(const glm::vec3& center);
    void SetEye(float x, float y, float z);
    void SetEye(const glm::vec3& eye);
    void SetUp(float x, float y, float z);
    void SetUp(const glm::vec3& up);
    
    void SetFocus(const glm::vec3& obj);

    glm::mat4 GetViewMaxtrix();
    glm::mat4 GetProjectionMatrix();
    void RecalculateViewMatrix(const glm::vec3 pos, const glm::vec3 targetPos);
    const glm::mat4 GetViewProjectionMatrix() const;

    glm::vec3 GetPosition() { return cameraPos; }

    void Update();
    void UpdateZoom();
    void UpdateViewMatrix();
    glm::mat4 CalculateSkyBoxViewProjection();

    glm::mat4 GetOrthoMatrix();


};



// Set camera position
//glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

// Need camera direction
// Example can be point to (0,0,0)
// can get position 
//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

// Need right axis
// A right vector that represents the positive x - axis of the camera space.
//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

// Need up axis
//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


// Look at
//Using right, up, direction vector and camera position

//glm::mat4 view = glm::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
