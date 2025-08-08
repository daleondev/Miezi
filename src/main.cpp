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

    const auto v1 = mz::Vec3().fillRandom().normalize();
    const auto v2 = mz::Vec3().fillRandom().normalize();

    const auto r1 = v1.rotationTo(v2);
    const auto r2 = v1.rotationTo_alt(v2);

    v1.print();
    v2.print();
    r1.print();    
    r2.print(); 

    MZ_TRACE("{}", v1.isFinite());
    MZ_TRACE("{}", v1.hasNaN());

    MZ_TRACE("{}", r1.isFinite());
    MZ_TRACE("{}", r1.hasNaN());
    MZ_TRACE("{}", r1.asMat4().isAffine());

    const auto m = mz::Mat4(1.0f).fillRandom().rescale();
    m.print();

    m.inverted().transposed().print();
    m.invertedTransposed().print();

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0; 
}