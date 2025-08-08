#include "mz/core/core.h"
// #include "mz/util/geometry.h"

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

    mz::Vec4 v1;
    v1.fillRandom();
    v1.print();
    MZ_INFO("{}", v1->r);

    mz::Vec4 v2(3.0f);
    auto v3 = v1 + v2;
    v3.print();

    // mz::Mat4 m1(1.0f);
    // m1.print();
    // m1.fillRandom();
    // m1.print();
    // m1.rescale().print();
    // MZ_INFO("{}", m1.maxValue());

    // m1.toEulerZYX().print();
    // m1.toEulerXYZ().print();

    // auto m2 = m1.asMat3();
    // m2.print();

    // m2.toEulerXYZ().print();

    // auto m3 = m1.asMat4();
    // m3.print();

    // glm::mat4 test(glm::mat3(10.0f));
    // mz::Mat4(test).print();

    // mz::Mat3 test2(10.0f);
    // test2.asMat4().print();

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0;
}