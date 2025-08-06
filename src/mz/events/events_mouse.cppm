module;
#include "event_macros.h"
export module mz.events.mouse;

import std;
import glm;

import mz.events;

namespace mz { 

    class MouseEnterEvent : public EventBase
    {
    public:
        MouseEnterEvent() = default;

        EVENT_CLASS_TYPE(MouseEnterEvent)
        EVENT_CLASS_CATEGORY(EventCategory::Mouse)
    };

    class MouseLeaveEvent : public EventBase
    {
    public:
        MouseLeaveEvent() = default;

        EVENT_CLASS_TYPE(MouseLeave)
        EVENT_CLASS_CATEGORY(EventCategory::Mouse)
    };

    class MouseMovedEvent : public EventBase 
    {
    public:
        MouseMovedEvent(const float x, const float y) : m_position(x, y) {}

        const glm::vec2& getPosition() const { return m_position; }
        float getX() const { return m_position.x; }
        float getY() const { return m_position.y; }

        std::string toString() const override
        {
            return std::format("{}: {}, {}", getName(), m_position.x, m_position.y);
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(static_cast<std::uint8_t>(EventCategory::Mouse) | static_cast<std::uint8_t>(EventCategory::Input))

    private:
        glm::vec2 m_position;

    };

    class MouseScrolledEvent : public EventBase 
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset) : m_offsets(xOffset, yOffset) {}

        const glm::vec2& getOffsets() const { return m_offsets; }
        float getXOffset() const { return m_offsets.x; }
        float getYOffset() const { return m_offsets.y; }

        std::string toString() const override
        {
            return std::format("{}: {}, {}", getName(), m_offsets.x, m_offsets.y);
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(static_cast<std::uint8_t>(EventCategory::Mouse) | static_cast<std::uint8_t>(EventCategory::Input))

    private:
        glm::vec2 m_offsets;

    };

    class MouseButtonEvent : public EventBase 
    {
    public:
        int getMouseButton() const { return m_button; }

        std::string toString() const override
        {
            return std::format("{}: {}", getName(), m_button);
        }

        EVENT_CLASS_CATEGORY(static_cast<std::uint8_t>(EventCategory::Mouse) | static_cast<std::uint8_t>(EventCategory::Input) | static_cast<std::uint8_t>(EventCategory::MouseButton))

    protected:
        MouseButtonEvent(const int button) : m_button(button) {}

        int m_button;

    };

    class MouseButtonPressedEvent : public MouseButtonEvent 
    {
    public:
        MouseButtonPressedEvent(const int button) : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonPressed)

    };

    class MouseButtonReleasedEvent : public MouseButtonEvent 
    {
    public:
        MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonReleased)

    };

    class MouseDroppedEvent : public EventBase
    {
    public:
        MouseDroppedEvent(const std::vector<std::string>& paths) : m_paths(paths) {}

        std::vector<std::string> getPaths() const { return m_paths; }
        std::size_t getNumPaths() const { return m_paths.size(); }
        std::string getPath(const std::size_t idx) const { return m_paths[idx]; }

        std::vector<std::string>::const_iterator cbegin() const { return m_paths.cbegin(); }
        std::vector<std::string>::const_iterator cend() const { return m_paths.cend(); }
        std::vector<std::string>::iterator begin() { return m_paths.begin(); }
        std::vector<std::string>::iterator end() { return m_paths.end(); }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << getName() << ":";
            for (std::size_t i = 0; i < m_paths.size(); ++i)
                ss << "\n\tPath_" << i << ": " << m_paths[i];
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseDropped)
        EVENT_CLASS_CATEGORY(EventCategory::Mouse)

    private:
        std::vector<std::string> m_paths;

    };

}