/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Context.h"
#include "Src/Inputs/Inputs.h"

#include <iostream>

namespace Eos
{

    Context::Context() :
        m_window{ nullptr },
        m_size{ glm::ivec2{0,0} },
        inRelease{ false },
        isCreated{ true },
        scr_height{ 0 },
        scr_width{ 0 },
        fps(0),
        wposx{ 0 },
        wposy{ 0 },
        posY{ 0 },
        posX{ 0 },
        cursor_x{0.0},
        cursor_y{0.0},
        scrollDelta{0.0},
        isGammaOn{false}
    {
    }

    Context* Context::Self()
    {
        Context* instance = (Context*)(glfwGetWindowUserPointer(m_window));
        assert(instance != nullptr);
        return instance;
    }

    void Context::Init(uint32_t wsizex, uint32_t wsizey)
    {

        if (!glfwInit())
        {
            exit(EXIT_SUCCESS);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(wsizex, wsizey, "EosEngine", NULL, NULL);

        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create GLFW Window !");
        }
        glfwSetWindowUserPointer(m_window, const_cast<Context*>(this));
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(0);





        glfwSetFramebufferSizeCallback(m_window, framebufferSize_fn);
        glfwSetScrollCallback(m_window, scroll_fn);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
        {
            std::cout << "glew not intialized" << std::endl;
        }


#if _DEBUG
        glDebugMessageCallback(GLUtils::debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
            GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
#endif

        scr_width = m_size.x = wsizex;
        scr_height = m_size.y = wsizey;
        isCreated = true;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        Eos::PE_SUBSCRIBE_EVENT
        (
            Eos::ResizeFrameBufferEvent,
            [this](Eos::ResizeFrameBufferEvent& _evn)
            {
                scr_width = (int32_t)_evn.size.x;
                scr_height = (int32_t)_evn.size.y;
                wposx = (int32_t)_evn.position.x;
                wposy = (int32_t)_evn.position.y;
            }
        );

    }

    void Context::Update()
    {

        glViewport(0, 0, scr_width, scr_height);
        glfwPollEvents();

        if (Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::F9))
        {
            isGammaOn = true;
        }
        if (Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::F10))
        {
            isGammaOn = false;
        }
        
    }

    void Context::PostUpdate()
    {
        scrollDelta = 0;
        auto [cx, cy] = cursorPosition();
        cursor_x = cx;
        cursor_y = cy;
        glfwSwapBuffers(m_window);
    }

    void Context::framebufferSize_fn(GLFWwindow* window, int x, int y)
    {
        auto instance = (Context*)(glfwGetWindowUserPointer(window));
        instance->framebufferCallback(x, y);
    }

    void Context::scroll_fn(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto instance = (Context*)(glfwGetWindowUserPointer(window));
        instance->mouseScrollCallback(xoffset, yoffset);
    }

    void Context::framebufferCallback(int width, int height)
    {
        glViewport(0, 0, width, height);
        assert(m_window != nullptr);
    }

    void Context::mouseScrollCallback(double xoffset, double yoffset)
    {
        UNREFERENCED_PARAMETER(xoffset);
        scrollDelta = yoffset;
        assert(m_window != nullptr);
    }

    double Context::scrollData()
    {
        return scrollDelta;
    }

    std::pair<double, double> Context::cursorPosition() const noexcept
    {
        double mousePositionX, mousePositionY;
        glfwGetCursorPos(m_window, &mousePositionX, &mousePositionY);
        return std::pair<double, double>(mousePositionX, mousePositionY);
    }

    std::pair<double, double> Context::cursorDelta() const
    {
        auto [mousex, mousey] = cursorPosition();
        return { mousex - cursor_x,mousey - cursor_y };
    }

    std::pair<double, double> Context::cursorNDCDelta() const
    {
        auto [mousex, mousey] = cursorPosition();
        return { mousex - cursor_x,mousey - cursor_y };
    }


    Context::~Context()
    {
        Destroy();
    }

    void Context::Destroy()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }


}