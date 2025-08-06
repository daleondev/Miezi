module;
#include "event_macros.h"
export module mz.events.window;

import std;
import glm;

import mz.events;

namespace mz { 

    export class WindowResizeEvent : public EventBase 
    {
    public:
        WindowResizeEvent(const float width, const float height) : m_size{width, height} {}

        const glm::vec2& getSize() const { return m_size; }
        float getWidth() const { return m_size.x; }
        float getHeight() const { return m_size.y; }        

        std::string toString() const override
        {
            return std::format("{}: {}, {}", getName(), m_size.x, m_size.y);
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::Window)

    private:
        glm::vec2 m_size;

    };

    export class WindowCloseEvent : public EventBase
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategory::Window)
    };

}