module;

#include "mz/core/core.h"

#include <GLFW/glfw3.h>

#define NATIVE_GLFW_WINDOW reinterpret_cast<GLFWwindow*>(m_window->getNativeWindow())

export module mz.graphics.window.glfw;

import std;
import glm;

import mz.core.types;
import mz.core.logging;

import mz.events;
import mz.events.window;
import mz.events.key;
import mz.events.mouse;

import mz.graphics.window;
import mz.graphics.renderer.opengl;

namespace mz { 

    export class GlfwGlContext : public GlRenderContext
    {
    public:
        GlfwGlContext(IWindow* window) : GlRenderContext(window) { }
        ~GlfwGlContext() = default;

        void makeCurrent() override
        {
            MZ_ASSERT(NATIVE_GLFW_WINDOW, "Window for gl context not set");
            glfwMakeContextCurrent(NATIVE_GLFW_WINDOW);
        }

        void init()
        {
            GlRenderContext::init(glfwGetProcAddress);
        }
    };

    export class GlfwInput : public InputBase
    {
    public:
        GlfwInput(IWindow* window) : InputBase(window) { }
        ~GlfwInput() = default;
        
        bool isKeyPressed(const int key) const override 
        {
            MZ_ASSERT(m_window, "Window for input not set");
            return glfwGetKey(NATIVE_GLFW_WINDOW, key) == GLFW_PRESS; 
        }

        bool isMousePressed(const int button) const override 
        {
            MZ_ASSERT(m_window, "Window for input not set");
            return glfwGetMouseButton(NATIVE_GLFW_WINDOW, button) == GLFW_PRESS; 
        }

        glm::vec2 getMousePosition() const override
        {
            MZ_ASSERT(m_window, "Window for input not set");

            double xpos, ypos;
		    glfwGetCursorPos(NATIVE_GLFW_WINDOW, &xpos, &ypos);
		    return { static_cast<float>(xpos), static_cast<float>(ypos) };
        }
    };

    export class GlfwWindow : public WindowBase
    {
    public:
        GlfwWindow(const std::string& title, const glm::vec2& size) 
            : WindowBase(title, size, std::make_unique<GlfwGlContext>(this), std::make_unique<GlfwInput>(this)), m_window{ nullptr }
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

            m_window = std::unique_ptr<GLFWwindow, WindowDeleter>(glfwCreateWindow(m_data.size.x, m_data.size.y, m_data.title.c_str(), nullptr, nullptr), WindowDeleter{});
            glfwSetWindowUserPointer(m_window.get() , &m_data);
            glfwMakeContextCurrent(m_window.get());
            
            m_context->makeCurrent();
            m_context->asPtrUnchecked<GlfwGlContext>()->init();

            setVSync(true);
            setupCallbacks();

            MZ_INFO("Created Window with title: {} and size: {}, {}", m_data.title, m_data.size.x, m_data.size.y);
        }

        void close() override
        {
            m_window.reset();

            if (--m_windowCount == 0)
                glfwTerminate();

            MZ_WARN("Destroyed Window with title: {}", m_data.title);
        }

        void update() override
        {
            glfwPollEvents();
            glfwSwapBuffers(m_window.get());
        }

        void setVSync(const bool enabled) override
        {
            (m_data.vSync = enabled) ? glfwSwapInterval(1) : glfwSwapInterval(0);
        }

        void* getNativeWindow() const override
        {
            return m_window.get();
        }

    private:
        void setupCallbacks()
        {
            glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.size.x = static_cast<float>(width);
                data.size.y = static_cast<float>(height);

                WindowResizeEvent event(width, height);
                data.eventCallback(&event);
            });

            glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow* window) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

                WindowCloseEvent event;
                data.eventCallback(&event);
            });

            glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
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

            glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int mods) -> void {
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

            glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xOffset, double yOffset) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                data.eventCallback(&event);
            });

            glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPos, double yPos) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                data.eventCallback(&event);
            });

            glfwSetCursorEnterCallback(m_window.get(), [](GLFWwindow* window, int entered) -> void {
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

            glfwSetDropCallback(m_window.get(), [](GLFWwindow* window, int pathCount, const char* cpaths[]) -> void {
                auto& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                DynamicArray<std::string> paths(pathCount);
                for (int i = 0; i < pathCount; ++i) {
                    paths[i] = cpaths[i];
                }

                MouseDroppedEvent event(&paths);
                data.eventCallback(&event);
            });
        }

        struct WindowDeleter 
        {
            void operator()(GLFWwindow* window) { glfwDestroyWindow(window); }
        };
        std::unique_ptr<GLFWwindow, WindowDeleter> m_window;

        inline static std::size_t m_windowCount = 0;
        
    };

}