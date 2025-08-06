#include "mz/core/core.h"

import std;
import glm;

import mz.window;
import mz.window.glfw;

import mz.events;
import mz.events.window;

import mz.graphics.render;
import mz.graphics.opengl.render;

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

    std::unique_ptr<mz::RenderBase> renderer = std::make_unique<mz::GlRenderer>(window->getContext().get());

    while(running) {
        window->update();
        renderer->clear(glm::vec4(1.0f));
    }
}