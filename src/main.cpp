#include "mz/core/core_defines.h"

import std;
import glm;

import mz.window;
import mz.window.glfw;

import mz.events;
import mz.events.window;

import mz.core.logging;

static bool running = true;

int main()
{
    std::unique_ptr<mz::IWindow> window = std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});
    window->setEventCallbackFunc([](mz::IEvent* e) 
    {
        if (e->is<mz::WindowCloseEvent>())
            running = false;
    });

    while(running) {
        window->update();
    }
}