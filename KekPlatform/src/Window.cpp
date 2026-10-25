//
// Created by Dmitriy on 12.10.2026.
//

#include "KekPlatform/Window.hpp"

#include <stdexcept>

#include <GLFW/glfw3.h>
#include "KekInput/Input.hpp"
namespace Kek::Platform
{
    namespace
    {
        // GLFW is initialized by the first window and terminated with the last one.
        int liveWindows = 0;

        void* LoadProc(const char* name)
        {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        }

        Input::Key ToKey(const int glfwKey)
        {
            using Input::Key;

            // Letters, digits and F-keys are contiguous both in GLFW and in Kek::Input::Key.
            if (glfwKey >= GLFW_KEY_A && glfwKey <= GLFW_KEY_Z)
                return static_cast<Key>(static_cast<int>(Key::A) + (glfwKey - GLFW_KEY_A));

            if (glfwKey >= GLFW_KEY_0 && glfwKey <= GLFW_KEY_9)
                return static_cast<Key>(static_cast<int>(Key::Num0) + (glfwKey - GLFW_KEY_0));

            if (glfwKey >= GLFW_KEY_F1 && glfwKey <= GLFW_KEY_F12)
                return static_cast<Key>(static_cast<int>(Key::F1) + (glfwKey - GLFW_KEY_F1));

            switch (glfwKey)
            {
                case GLFW_KEY_UP: return Key::Up;
                case GLFW_KEY_DOWN: return Key::Down;
                case GLFW_KEY_LEFT: return Key::Left;
                case GLFW_KEY_RIGHT: return Key::Right;
                case GLFW_KEY_SPACE: return Key::Space;
                case GLFW_KEY_ENTER: return Key::Enter;
                case GLFW_KEY_ESCAPE: return Key::Escape;
                case GLFW_KEY_TAB: return Key::Tab;
                case GLFW_KEY_BACKSPACE: return Key::Backspace;
                case GLFW_KEY_LEFT_SHIFT: return Key::LeftShift;
                case GLFW_KEY_RIGHT_SHIFT: return Key::RightShift;
                case GLFW_KEY_LEFT_CONTROL: return Key::LeftCtrl;
                case GLFW_KEY_RIGHT_CONTROL: return Key::RightCtrl;
                case GLFW_KEY_LEFT_ALT: return Key::LeftAlt;
                case GLFW_KEY_RIGHT_ALT: return Key::RightAlt;
                default: return Key::Unknown;
            }
        }

        void OnKey(GLFWwindow*, const int key, const int, const int action, const int)
        {
            if (action == GLFW_REPEAT) return; // Input tracks held state itself

            Input::OnKeyEvent(ToKey(key), action == GLFW_PRESS);
        }
    }

    Window::Window(const WindowSettings& settings) : settings(settings)
    {
        if (liveWindows == 0 && !glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        glfwWindow = glfwCreateWindow(
            static_cast<int>(settings.width),
            static_cast<int>(settings.height),
            settings.title.c_str(),
            nullptr,
            nullptr
        );

        if (glfwWindow == nullptr)
        {
            if (liveWindows == 0) glfwTerminate();
            throw std::runtime_error("Failed to create a window (OpenGL 3.3 core is required)");
        }
        glfwSetKeyCallback(glfwWindow, &OnKey);
        ++liveWindows;
    }

    Window::~Window()
    {
        glfwDestroyWindow(glfwWindow);

        if (--liveWindows == 0) glfwTerminate();
    }

    Window::GLProcLoader Window::GetProcLoader() const
    {
        return &LoadProc;
    }

    void Window::MakeContextCurrent() const
    {
        glfwMakeContextCurrent(this->glfwWindow);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(this->glfwWindow);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(this->glfwWindow);
    }

    FramebufferSize Window::GetFramebufferSize() const
    {
        FramebufferSize size{};
        glfwGetFramebufferSize(this->glfwWindow, &size.width, &size.height);
        return size;
    }
}