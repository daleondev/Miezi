#include "mz/core/core.h"
#include <GLFW/glfw3.h>

import std;
import glm;

import mz.core.types;
import mz.core.logging;

import mz.events;
import mz.events.window;
import mz.events.mouse;

import mz.graphics.window;
import mz.graphics.renderer;
import mz.graphics.renderer.opengl;
import mz.graphics.renderer.camera;

import mz.util;
import mz.util.time;
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

std::shared_ptr<ICameraController> camCtrl;

int main()
{
    auto window = mz::WindowBase::create("Test Window", glm::vec2{800, 600});
    
    // std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});
    window->setEventCallbackFunc([](mz::IEvent* e) 
    {
        if (e->is<mz::WindowCloseEvent>())
            running = false;

        auto camera = std::static_pointer_cast<OrbitCameraController>(camCtrl);
        if (e->is<MouseButtonPressedEvent>()) {
            if (e->asPtr<MouseButtonPressedEvent>()->getMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
                camCtrl->startDraggingRot();
        }
        else if (e->is<MouseButtonReleasedEvent>()) {
            if (e->asPtr<MouseButtonReleasedEvent>()->getMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
                camCtrl->stopDraggingRot();
        }
    });

    auto renderer = RenderBase::create(window->getContext().get());

    std::shared_ptr<ICamera> cam = std::make_shared<PerspectiveCamera>(degToRad(60.0f), window->getSize().x / window->getSize().y, 0.001f, 100.0f);
    camCtrl = std::make_shared<OrbitCameraController>(cam);

    auto prevTime = std::chrono::high_resolution_clock::now();

    while(running) {
        const auto currTime = std::chrono::high_resolution_clock::now();
        const auto timeDiff = currTime - prevTime;
        prevTime = currTime;

        Timestep dt(std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count() / 1000.0f);

        window->update(); 
        camCtrl->update(dt, window->getInput().get());

        // cam.setRotation(cam.getRotation() + Vec3{degToRad(1.0f), 0.0f, 0.0f});
        renderer->clear(Vec4(1.0f));
        renderer->drawBox(cam.get(), Mat4(1.0f), {1.0f, 0.0f, 0.0f, 1.0f});
    }

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0; 
}