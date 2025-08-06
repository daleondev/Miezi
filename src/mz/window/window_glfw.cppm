module;
#include <GLFW/glfw3.h>

#define MZ_UNUSED(x) (void)x
export module mz.window.glfw;

import std;
import glm;

import mz.window;
import mz.events;
import mz.events.window;
import mz.events.key;
import mz.events.mouse;

namespace mz { 

    export class GlfwGlContext : public IGraphicsContext
    {
    public:
        GlfwGlContext(GLFWwindow* window)
            : m_window{ window } {}
        ~GlfwGlContext() = default;

        void makeCurrent() override
        {
            if (m_window)
                glfwMakeContextCurrent(m_window);
        }

        void swapBuffers() override
        {
            if (m_window)
                glfwSwapBuffers(m_window);
        }

    private:
        GLFWwindow* m_window;
        
    };

    export class GlfwInput : public IInput
    {
    public:
        GlfwInput(GLFWwindow* window)
            : m_window{ window } {}
        ~GlfwInput() = default;
        
        bool isKeyPressed(const int key) const override { return glfwGetKey(m_window, key) == GLFW_PRESS; }

        bool isMousePressed(const int button) const override { return glfwGetMouseButton(m_window, button) == GLFW_PRESS; }

        glm::vec2 getMousePosition() const override
        {
            double xpos, ypos;
		    glfwGetCursorPos(m_window, &xpos, &ypos);

		    return { static_cast<float>(xpos), static_cast<float>(ypos) };
        }
        
    private:
        GLFWwindow* m_window;

    };

    export class GlfwWindow : public WindowBase
    {
    public:
        GlfwWindow(const std::string& title, const glm::vec2& size) 
            : WindowBase(title, size, std::make_unique<GlfwGlContext>(m_window), std::make_unique<GlfwInput>(m_window))
        {
            open();
        }

        ~GlfwWindow()
        {
            close();
        }

        void open() override
        {
            if (m_windowCount++ == 0)
                glfwInit();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);

            m_window = glfwCreateWindow(m_data.size.x, m_data.size.y, m_data.title.c_str(), nullptr, nullptr);
            glfwSetWindowUserPointer(m_window, &m_data);
            glfwMakeContextCurrent(m_window);
            setVSync(true);

            std::println("OpenGL:");
            std::println("\tOpenGL Vendor: {}", (char*)glGetString(GL_VENDOR));
            std::println("\tOpenGL Renderer: {}", (char*)glGetString(GL_RENDERER));
            std::println("\tOpenGL Version: {}", (char*)glGetString(GL_VERSION));
            std::println("\tGLSL Version: {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

            setupCallbacks();
        }

        void close() override
        {
            glfwDestroyWindow(m_window);

            if (--m_windowCount == 0)
                glfwTerminate();
        }

        void update() override
        {
            glfwPollEvents();
            m_context->swapBuffers();
        }

        void setVSync(const bool enabled) override
        {
            (m_data.vSync = enabled) ? glfwSwapInterval(1) : glfwSwapInterval(0);
        }

        void* getNativeWindow() const override
        {
            return m_window;
        }

    private:
        void setupCallbacks()
        {
            glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.size.x = static_cast<float>(width);
                data.size.y = static_cast<float>(height);

                WindowResizeEvent event(width, height);
                data.eventCallback(&event);
            });

            glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

                WindowCloseEvent event;
                data.eventCallback(&event);
            });

            glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
                MZ_UNUSED(scancode);
                MZ_UNUSED(mods);

                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                switch(action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(key, 0);
                        data.eventCallback(&event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(key);
                        data.eventCallback(&event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent event(key, 1);
                        data.eventCallback(&event);
                        break;
                    }
                }
            });

            glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) -> void {
                MZ_UNUSED(mods);

                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                switch(action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(button);
                        data.eventCallback(&event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(button);
                        data.eventCallback(&event);
                        break;
                    }
                }
            });

            glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                data.eventCallback(&event);
            });

            glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                data.eventCallback(&event);
            });

            glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                if (entered) {
                    MouseEnterEvent event;
                    data.eventCallback(&event);
                }
                else {
                    MouseLeaveEvent event;
                    data.eventCallback(&event);
                }
            });

            glfwSetDropCallback(m_window, [](GLFWwindow* window, int pathCount, const char* cpaths[]) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                std::vector<std::string> paths(pathCount);
                for (int i = 0; i < pathCount; ++i) {
                    paths[i] = cpaths[i];
                }

                MouseDroppedEvent event(paths);
                data.eventCallback(&event);
            });
        }

        GLFWwindow* m_window;

        inline static std::size_t m_windowCount = 0;
        
    };

}