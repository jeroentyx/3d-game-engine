#include "EditorCamera.h"

namespace editor
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        position(),
        cameraFront(),
        cameraUp(),
        cameraRight(),
        worldUp(),
        cameraYaw(),
        cameraPitch(),
        cameraSpeed(),
        cameraSensitivity(),
        cameraFov()
    {
        //
        //	One small step at a time, focus on incremental improvement
        //
        this->position = position;
        this->worldUp = up;
        this->cameraYaw = yaw;
        this->cameraPitch = pitch;
        this->UpdateCameraMatrix();

    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(
            position,
            position + cameraFront,
            cameraUp);
    }

    void Camera::GlfwKeyboardCall(CameraMovement direction, float deltaTime)
    {
        //
        //  Going to pass the data in by events
        //
        GLfloat camVelocity = this->cameraSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD)
        {
            position += cameraFront * camVelocity;
        }
        if (direction == CameraMovement::BACKWARD)
        {
            position -= cameraFront * camVelocity;
        }
        if (direction == CameraMovement::LEFT)
        {
            position -= cameraFront * camVelocity;
        }
        if (direction == CameraMovement::RIGHT)
        {
            position += cameraFront * camVelocity;
        }
    }

    void Camera::GlfwMouseCall(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= cameraSensitivity;
        yoffset *= cameraSensitivity;
        cameraYaw += xoffset;
        cameraPitch += yoffset;

        //Make the pitch in the range of 89.0f and -89.0f;

        if (constrainPitch)
        {
            cameraPitch = (cameraPitch > 89.0f) ?
                89.0f : -89.0f;
        }
        this->UpdateCameraMatrix();

    }

    void Camera::GlfwScrollCall(float yoffset)
    {
        //check if cameraFov is within the range of 1.0f and 45.0f

        if (cameraFov >= glm::radians(1.0f) && cameraFov <= glm::radians(45.0f))
        {
            cameraFov -= glm::radians(yoffset);
        }

        cameraFov = (cameraFov <= glm::radians(1.0f)) ? glm::radians(1.0f) : glm::radians(45.0f);

    }

    void Camera::UpdateCameraMatrix()
    {
        glm::vec3 front;
        front.x = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

        //Right hand rule if you cross front and up you will get the right
        //if you cross right and front you get the worldUp
        cameraFront = glm::normalize(front);
        cameraRight = glm::normalize(glm::cross(cameraFront,this->worldUp));
        cameraUp = glm::normalize(glm::cross(this->cameraRight, cameraFront));

    }

    Camera::~Camera()
    {
        //
        //  didn't allocate any resources , empty for now
        //
    }
}