#include "mz/core/core.h"
// #include "mz/util/geometry.h"

import std;
import glm;

import mz.core.types;
import mz.core.logging;

import mz.events;
import mz.events.window;

import mz.graphics.window;
import mz.graphics.renderer;
import mz.graphics.renderer.opengl;
import mz.graphics.renderer.camera;

import mz.util;
import mz.math.geometry;

using namespace mz;

static bool running = true;

class Test : public IIterable<int>
{
public:
    Test()
    {
        m_ints = {
            11,
            -2542,
            4367435
        };
    }

    ITERABLE_CONTAINER(m_ints, int);

private:
    std::vector<int> m_ints;
};

template<typename T>
concept Range = std::ranges::range<T>;

template<typename T>
void printRange(const T& range)
{
    std::stringstream ss;
    for (std::size_t i = 0; i < range.size(); ++i)
        ss << "\n" << i << ": " << range[i];

    MZ_TRACE("{}", ss.str());
}

int main()
{
    auto window = mz::WindowBase::create("Test Window", glm::vec2{800, 600});
    
    // std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});
    window->setEventCallbackFunc([](mz::IEvent* e) 
    {
        if (e->is<mz::WindowCloseEvent>())
            running = false;
    });

    auto renderer = RenderBase::create(window->getContext().get());

    OrthoCamera cam(-1, 1, -1, 1);
    cam.setPosition({0.0f, 0.0f, -2.0f});

    while(running) {
        window->update();
        renderer->clear(Vec4(1.0f));
        renderer->drawPoint(&cam, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
    }

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0; 
}