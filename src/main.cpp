#include "mz/core/core.h"

import std;
import glm;

import mz.core.logging;

import mz.events;
import mz.events.window;

import mz.graphics.window;
import mz.graphics.renderer;
import mz.graphics.renderer.opengl;

import mz.util.geometry;

static bool running = true;

void test(glm::vec2 t)
{
    MZ_INFO("{}", glm::gtx::to_string(t));
}

int main()
{
    // auto window = mz::WindowBase::create("Test Window", glm::vec2{800, 600});
    
    // // std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});
    // window->setEventCallbackFunc([](mz::IEvent* e) 
    // {
    //     if (e->is<mz::WindowCloseEvent>())
    //         running = false;
    // });

    // std::unique_ptr<mz::RenderBase> renderer = std::make_unique<mz::GlRenderer>(window->getContext().get());

    // while(running) {
    //     window->update();
    //     renderer->clear(glm::vec4(1.0f));
    // }

    glm::mat4 t1(1.0f);

    t1 = glm::gtx::translate(t1, glm::vec3(10.0f, 2.0f, -4.0f));
    mz::Mat4 m(t1);

    mz::Vec4 pos = m.translation();
    pos.print();

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0;
}