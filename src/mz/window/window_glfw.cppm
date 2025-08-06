module;
#include <GLFW/glfw3.h>
export module mz.window.glfw;

import std;
import glm;

import mz.window;

namespace mz { 

    export class GlfwWindow : public WindowBase
    {
    public:

    private:
    };

    export class GlfwInput : public IInput
    {
    public:
        void foo() override { }
    private:
    };

}