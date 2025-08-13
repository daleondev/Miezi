#include "mz/core/core.h"
#include <GLFW/glfw3.h>

import std;
import glm;

import mz.core.types;
import mz.core.logging;

import mz.events;
import mz.events.window;
import mz.events.key;

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

std::shared_ptr<ICamera> cam;

int main()
{
    auto window = mz::WindowBase::create("Test Window", glm::vec2{800, 600});
    
    // std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});
    window->setEventCallbackFunc([](mz::IEvent* e) 
    {
        if (e->is<mz::WindowCloseEvent>())
            running = false;

        if (e->is<KeyPressedEvent>()) {
            auto camera = std::static_pointer_cast<PerspectiveCamera>(cam);
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_W)
                camera->setRotation(camera->getRotation() + Vec3{degToRad(1.0f), 0.0f, 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_S)
                camera->setRotation(camera->getRotation() + Vec3{degToRad(-1.0f), 0.0f, 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_A)
                camera->setRotation(camera->getRotation() + Vec3{0.0f, degToRad(1.0f), 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_D)
                camera->setRotation(camera->getRotation() + Vec3{0.0f, degToRad(-1.0f), 0.0f});

            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_UP)
                camera->setPosition(camera->getPosition() + Vec3{0.0f, 1.0f, 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_DOWN)
                camera->setPosition(camera->getPosition() + Vec3{0.0f, -1.0f, 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_LEFT)
                camera->setPosition(camera->getPosition() + Vec3{-1.0f, 0.0f, 0.0f});
            if (e->asPtr<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_RIGHT)
                camera->setPosition(camera->getPosition() + Vec3{1.0f, 0.0f, 0.0f});
        }
    });

    auto renderer = RenderBase::create(window->getContext().get());

    cam = std::make_shared<PerspectiveCamera>(degToRad(60.0f), window->getSize().x / window->getSize().y, 0.001f, 100.0f);
    std::static_pointer_cast<PerspectiveCamera>(cam)->setPosition({0.0f, 0.0f, 2.0f});

    while(running) {
        window->update(); 
        // cam.setRotation(cam.getRotation() + Vec3{degToRad(1.0f), 0.0f, 0.0f});
        renderer->clear(Vec4(1.0f));
        renderer->drawBox(cam.get(), Mat4(1.0f), {1.0f, 0.0f, 0.0f, 1.0f});
    }

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0; 
}