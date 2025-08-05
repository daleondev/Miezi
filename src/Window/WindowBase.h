#pragma once

#include "IWindow.h"

class WindowBase : public IWindow
{
public:
    virtual void update() { }

    virtual glm::vec2& getSize() = 0;
    virtual void* getNativeWindow() = 0;

    virtual void setVSync(const bool enabled) = 0;
	virtual bool getVSync() = 0;

protected:
    struct WindowData {
        std::string title;
        glm::vec2 size;
        bool vSync;

        EventCallbackFunction eventCallback;
    };
    WindowData m_data;
};