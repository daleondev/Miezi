export module mz.window;

import std;
import glm;

namespace mz { 

    export class IWindow
    {
    public:
        virtual bool open(const std::string& title, const glm::vec2& size) = 0;
        virtual void close() = 0;

        virtual void update() = 0;

        virtual glm::vec2& getSize() = 0;
        virtual void* getNativeWindow() = 0;
        virtual bool getVSync() = 0;
    };

    export class WindowBase : public IWindow
    {
    public:
      
    protected:
    };

    export class IInput
    {
    public:
        virtual ~IInput() = default;
        virtual void foo() = 0;
    };

}