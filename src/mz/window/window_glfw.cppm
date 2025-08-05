module;
#include <GLFW/glfw3.h>
export module mz.window.glfw;

import std;
import glm;

import mz.window;

namespace mz { 

    namespace glfw { 

        export class Window : public WindowBase
        {
        public:

        private:
        };

        export class Input : public IInput
        {
        public:
            void foo() override { }
        private:
        };

    }

}