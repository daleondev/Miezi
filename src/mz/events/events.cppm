export module mz.events;

import std;

import mz.core.types;

namespace mz { 

    export enum class EventType : std::uint8_t 
    {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased, KeyTyped,
        MouseEnterEvent, MouseLeave, MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseDropped
    };

    export enum class EventCategory : std::uint8_t 
    {
        None            = 0,
        Window          = (1 << 0),
        Input           = (1 << 1),
        Keyboard        = (1 << 2),
        Mouse           = (1 << 3),
        MouseButton     = (1 << 4),    
    };

    export class IEvent : public ICastable
    {
    public:
        virtual ~IEvent() = default;

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::uint8_t getCategoryFlags() const = 0;
        virtual std::string toString() const = 0;

        virtual bool isInCategory(EventCategory category) = 0;

        virtual bool isHandled() const = 0;
        virtual void setHandled(bool handled) = 0;
    };

    export class EventBase : public IEvent
    {
    public:
        virtual ~EventBase() = default;

        virtual std::string toString() const override { return getName(); }

        bool isInCategory(EventCategory category) override { return getCategoryFlags() & static_cast<std::uint8_t>(category); }

        bool isHandled() const override { return m_handled; }
        void setHandled(bool handled) override { m_handled = handled; }

    private:
        bool m_handled = false;

    };

    export typedef std::function<void(IEvent*)> EventCallbackFunc;

    export class EventDispatcher 
    {
    public:
        EventDispatcher(IEvent* event) : m_event(event) {}

        template<typename T, typename F>
        bool dispatch(const F& func)
        {
            if (m_event->getEventType() == T::getStaticType()) {
                m_event->setHandled(m_event->isHandled() | func(m_event->asPtrUnchecked<T>()));
                return true;
            }
            return false;
        }

    private:
        IEvent* m_event;

    };

}