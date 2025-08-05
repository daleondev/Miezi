#pragma once

class IWindow
{
public:
    virtual bool open(const std::string& title, const glm::vec2& size) = 0;
    virtual void close() = 0;

    virtual void update() = 0;

    virtual glm::vec2& getSize() = 0;
    virtual void* getNativeWindow() = 0;
	virtual bool getVSync() = 0;
};