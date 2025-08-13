export module mz.graphics.window;

import std;
import glm;

import mz.core.types;
import mz.events;

import mz.graphics.renderer;

namespace mz { 

    export class IInput : public ICastable
    {
    public:
        virtual ~IInput() = default;

        virtual void update() = 0;
        
        virtual bool isKeyPressed(const int key) const = 0;

        virtual bool isMousePressed(const int button) const = 0;
        virtual glm::vec2 getMousePosition() const = 0;
        virtual glm::vec2 getMouseDelta() const = 0;
    };

    export class IWindow;
    export class InputBase : public IInput
    {
    public:
        InputBase(IWindow* window) : m_window{ window } { }
        virtual ~InputBase() = default;

        virtual void update() override
        {
            m_mousePosPrev = m_mousePos;
            m_mousePos = getMousePosition();
        }

        glm::vec2 getMouseDelta() const override
        {
		    return m_mousePos - m_mousePosPrev;
        }
        
    protected:
        IWindow* m_window;
        glm::vec2 m_mousePos;
        glm::vec2 m_mousePosPrev;
    };

    export class IWindow : public ICastable
    {
    public:
        virtual ~IWindow() = default;

        virtual void open() = 0;
        virtual void close() = 0;

        virtual void update() = 0;

        virtual const std::string& getTitle() const = 0;
        virtual const glm::vec2& getSize() const = 0;
        virtual bool getVSync() const = 0;
        virtual void setVSync(const bool enabled) = 0;
        virtual void setEventCallbackFunc(const EventCallbackFunc& callback) = 0;

        virtual std::unique_ptr<IRenderContext>& getContext() = 0;
        virtual const std::unique_ptr<IRenderContext>& getContext() const = 0;
        virtual std::unique_ptr<IInput>& getInput() = 0;
        virtual const std::unique_ptr<IInput>& getInput() const = 0;

        virtual void* getNativeWindow() const = 0;
    };

    export class WindowBase : public IWindow
    {
    public:
        static std::shared_ptr<WindowBase> create(const std::string& title, const glm::vec2& size);
        
        virtual ~WindowBase() = default;

        const std::string& getTitle() const override { return m_data.title; }
        const glm::vec2& getSize() const override { return m_data.size; }
        bool getVSync() const override { return m_data.vSync; }
        void setEventCallbackFunc(const EventCallbackFunc& callback) override { m_data.eventCallback = callback; }

        std::unique_ptr<IRenderContext>& getContext() override { return m_context; }
        const std::unique_ptr<IRenderContext>& getContext() const override { return m_context; }
        std::unique_ptr<IInput>& getInput() override { return m_input; }
        const std::unique_ptr<IInput>& getInput() const override { return m_input; }
      
    protected:
        WindowBase(const std::string& title, const glm::vec2& size, std::unique_ptr<IRenderContext>&& context, std::unique_ptr<IInput>&& input) 
            : m_data{ .title = title, .size = size, .vSync = false, .eventCallback = [](IEvent*) { } }, 
            m_context{ std::move(context) }, m_input{ std::move(input) } { }

        struct WindowData {
            std::string title;
            glm::vec2 size;
            bool vSync;

            EventCallbackFunc eventCallback;
        };
        WindowData m_data;

        std::unique_ptr<IRenderContext> m_context;
        std::unique_ptr<IInput> m_input;

    };

}