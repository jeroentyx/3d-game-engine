#include "PerspectiveCamera.h"
#include "Src/Inputs/Inputs.h"
#include "../Src//Engine/Engine/CoreManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

PerspectiveCamera::PerspectiveCamera() : 
    scr_width{ 0 }, 
    scr_height{ 0 }, 
    gamescreen_posX{ 0 },
    gamescreen_posY{ 0 },
    m_ProjectionMatrix{ glm::mat4{1.0f} },
    m_ViewProjectionMatrix{ glm::mat4{1.0f} }
{
    yaw = -89.0f;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    cameraRight.x = -cos(glm::radians(yaw));
    cameraRight.y = 0.0f;
    cameraRight.z = sin(glm::radians(yaw));

    cameraUp = glm::cross(cameraFront, cameraRight);

    // Normalize everything
    cameraFront = glm::normalize(cameraFront);
    cameraRight = glm::normalize(cameraRight);
    cameraUp = glm::normalize(cameraUp);

    // Projection from window height and width, 800 x 1280
    m_ProjectionMatrix = glm::perspective(glm::radians(20.0f),
        (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);

    m_ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    cameraSpeed = 0.5f; //TODO: Fix till here
                        //need to get the correct mouse delta
                        //once i get the correct mouse delta, translate the camera position
}

PerspectiveCamera::PerspectiveCamera(const glm::vec3& startingPosition) :scr_width{ 0 },
scr_height{ 0 },
gamescreen_posX{ 0 },
gamescreen_posY{ 0 },
m_ProjectionMatrix{ glm::mat4{1.0f} },
m_ViewProjectionMatrix{ glm::mat4(1.0f) }
{
    cameraPos = startingPosition;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    this->cameraRight = glm::normalize(glm::cross(this->cameraFront, this->worldUp));
    this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraFront));

    m_ProjectionMatrix = glm::perspective(glm::radians(fov),
        (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 1000.0f);

    m_ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

const glm::vec3& PerspectiveCamera::GetCenter()
{
    cameraCenter = cameraPos + cameraFront;
    return cameraCenter;
}

const glm::vec3& PerspectiveCamera::GetEye()
{
    return cameraPos;
}

const glm::vec3& PerspectiveCamera::GetUp()
{
    return cameraUp;
}


void PerspectiveCamera::SetCenter(float x, float y, float z)
{
    (void)x;
    (void)y;
    (void)z;
}

void PerspectiveCamera::SetCenter(const glm::vec3& center)
{
    cameraCenter = center;
}

void PerspectiveCamera::SetEye(float x, float y, float z)
{
    cameraPos = glm::vec3(x, y, z);
}

void PerspectiveCamera::SetEye(const glm::vec3& eye)
{
    cameraPos = eye;
}

void PerspectiveCamera::SetUp(float x, float y, float z)
{
    cameraUp = glm::vec3(x, y, z);
}

void PerspectiveCamera::SetUp(const glm::vec3& up)
{
    cameraUp = up;
}

glm::mat4 PerspectiveCamera::GetViewMaxtrix()
{
    //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return m_ViewMatrix;
}

void PerspectiveCamera::Init(glm::vec3 pos, glm::vec3 up, float _yaw, float _pitch)
{
    cameraPos = pos;
    worldUp = up;
    yaw = _yaw;
    pitch = _pitch;
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
    /*glm::mat4 proj = glm::perspective(glm::radians(20.0f),
        (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);*/
    return m_ProjectionMatrix;
}

void PerspectiveCamera::RecalculateViewMatrix(const glm::vec3 pos, const glm::vec3 targetPos)
{
    UpdateZoom();
    glm::vec3 Direction = glm::normalize(targetPos - pos);
    glm::vec3 Right = glm::normalize(glm::cross(worldUp, Direction));
    glm::vec3 Up = glm::normalize(glm::cross(Right, -Direction));
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
    m_ViewMatrix = glm::lookAt(pos, targetPos, Up);
}

const glm::mat4 PerspectiveCamera::GetViewProjectionMatrix() const
{
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 proj = glm::perspective(glm::radians(fov), (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
    return proj * view;
}

void PerspectiveCamera::Update()
{
    //float deltaTime = static_cast<float>(Eos::FrameRateManager::GetInstance().delta());
    // Camera process input for keyboard movement
    if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP || 
        CoreManager::GetInstance().getEngineState() == ENGINESTATE::PAUSE)
    {
        Eos::Inputs::GetInstance().SetCursorMode(Cursor::Normal);

        if (Eos::Inputs::GetInstance().IsKeyPressed(EOS_KEY_W))
            cameraPos += cameraSpeed * cameraFront;
        if (Eos::Inputs::GetInstance().IsKeyPressed(EOS_KEY_S))
            cameraPos -= cameraSpeed * cameraFront;
        if (Eos::Inputs::GetInstance().IsKeyPressed(EOS_KEY_A))
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (Eos::Inputs::GetInstance().IsKeyPressed(EOS_KEY_D))
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else
    {
        //Eos::Inputs::GetInstance().SetCursorMode(Cursor::Locked);
    }

    //
    //  Before modification
    // 
    //// Mouse movement
    //// Need to save cameraFront, position and up
    //auto pos = Eos::Inputs::GetInstance().GetMousePosition();
    //float xpos = pos.first;
    //float ypos = pos.second;
    //if (firstMouse)
    //{
    //	lastX = xpos;
    //	lastY = ypos;
    //	firstMouse = false;
    //}
    //
    //float xoffset = (xpos - lastX);
    //float yoffset = (lastY - ypos); // reversed since y-coordinates go from bottom to top
    //lastX = xpos;
    //lastY = ypos;
    //
    //float sensitivity = 0.1f;
    //xoffset *= sensitivity;
    //yoffset *= sensitivity;
    //
    //yaw += xoffset;
    //pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped

    auto [deltaMouseX, deltaMouseY] = Eos::Context::GetInstance().cursorDelta();
    deltaMouseY *= -1.0;

    float sensitivity = 0.1f;
    deltaMouseX *= sensitivity;
    deltaMouseY *= sensitivity;

    yaw += (float)deltaMouseX;
    pitch += (float)deltaMouseY;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calculate front
    glm::vec3 front;
    front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    cameraRight = glm::normalize(glm::cross(front, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    //// Calculate right
    //cameraRight.x = -cos(glm::radians(yaw));
    //cameraRight.y = 0.0f;
    //cameraRight.z = sin(glm::radians(yaw));

    //// Calculate up; needs to be saved instead of using world up all the time
    //cameraUp = glm::cross(cameraFront, cameraRight);

    //// Normalize everything
    //cameraFront = glm::normalize(cameraFront);
    //cameraRight = glm::normalize(cameraRight);
    //cameraUp = glm::normalize(cameraUp);
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
    m_ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void PerspectiveCamera::UpdateZoom()
{
    //
    // Before Modification
    //
    // Check scroll in and out
    //if (Eos::Inputs::GetInstance().direction == Eos::ScrollDirection::Up)
    //{
    //	if (fov > 1.0f)
    //		fov -= 1.0f;
    //	m_ProjectionMatrix = glm::perspective(glm::radians(fov),
    //		(float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
    //}
    //else if (Eos::Inputs::GetInstance().direction == Eos::ScrollDirection::Down)
    //{
    //	if (fov < 45.0f)
    //		fov += 1.0f;
    //	m_ProjectionMatrix = glm::perspective(glm::radians(fov),
    //		(float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
    //}
    //Eos::Inputs::GetInstance().direction = Eos::ScrollDirection::Still;

    //
    // Modification
    //
    float scrollOffset = static_cast<float>(Eos::Context::GetInstance().scrollData());
    fov -= scrollOffset;
    if (fov < 1.0f)
    {
        fov = 1.0f;
    }
    else if (fov > 60.0f)
    {
        fov = 60.0f;
    }

    fov = 60.0f;
}

void PerspectiveCamera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 PerspectiveCamera::CalculateSkyBoxViewProjection()
{
    glm::vec3 worldup = cameraUp;
    glm::vec3 center = cameraPos + cameraFront;
    

    glm::mat4 sview = glm::lookAt(cameraPos, cameraPos + cameraFront, worldup);
    return sview;
}

glm::mat4 PerspectiveCamera::GetOrthoMatrix()
{
    int32_t half_width = Eos::Context::GetInstance().scr_width / 2;
    int32_t half_height = Eos::Context::GetInstance().scr_height / 2;
    return glm::ortho(-(float)half_width, (float)half_width, -(float)half_height, (float)half_height);
}
