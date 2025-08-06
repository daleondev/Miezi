module;
#include "events.h"
export module mz.events.key;

import std;
import glm;

import mz.events;

namespace mz { 

    export class KeyEvent : public EventBase 
    {
    public:
        int getKeyCode() const { return m_keyCode; }

        virtual std::string toString() const override
        {
            return std::format("{}: {}", getName(), m_keyCode);
        }

        EVENT_CLASS_CATEGORY(static_cast<std::uint8_t>(EventCategory::Keyboard) | static_cast<std::uint8_t>(EventCategory::Input))

    protected:
        KeyEvent(const int keyCode) : m_keyCode(keyCode) {}

        int m_keyCode;

    };

    export class KeyPressedEvent : public KeyEvent 
    {
    public:
        KeyPressedEvent(const int keyCode, const std::uint16_t repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        std::uint16_t GetRepeatCount() const { return m_RepeatCount; }

        std::string toString() const override
        {
            return std::format("{}: {} ({} repeats)", getName(), m_keyCode, m_RepeatCount);
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        std::uint16_t m_RepeatCount;

    };

    export class KeyReleasedEvent : public KeyEvent 
    {
    public:
        KeyReleasedEvent(const int keyCode) : KeyEvent(keyCode) {}

        EVENT_CLASS_TYPE(KeyReleased)

    };

    export class KeyTypedEvent : public KeyEvent 
    {
    public:
        KeyTypedEvent(const int keycode) : KeyEvent(keycode) {}

        EVENT_CLASS_TYPE(KeyTyped)

    };

}