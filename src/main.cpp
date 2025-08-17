#include "mz/core/core.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

import std;
import glm;

import mz.core.types;
import mz.core.logging;

import mz.events;
import mz.events.window;
import mz.events.mouse;
import mz.events.key;

import mz.graphics.window;

import mz.graphics.renderer;
import mz.graphics.renderer.opengl;
import mz.graphics.renderer.camera;

import mz.graphics.scene;
import mz.graphics.scene.components;
import mz.graphics.scene.component_utils;

import mz.util;
import mz.util.time;
import mz.math.geometry;

using namespace mz;

static bool running = true;

int main()
{
    auto window = mz::WindowBase::create("Test Window", glm::vec2{800, 600});
    auto renderer = RenderBase::create(window->getContext().get());
    Scene scene(window->getSize(), renderer);
    
    window->setEventCallbackFunc([&](mz::IEvent* e) 
    {
        if (e->isHandled())
            return;

        if (e->is<mz::WindowCloseEvent>()) {
            running = false;
            return;
        }

        if (e->is<KeyPressedEvent>()) {
            if (e->as<KeyPressedEvent>()->getKeyCode() == GLFW_KEY_C) {
                scene.swapCameraType();
                return;
            }
        }

        scene.onEvent(e);
    });

    // ----- prepare Axes -----
    
    // X
    auto xAxis = scene.createEntity("xAxis");
    xAxis.addComponent<LineRendererComponent>(Vec4(1.0f, 0.0f, 0.0f, 1.0f), 2.0f);
    setTransform(xAxis.getComponent<TransformComponent>(), Mat4(1.0f).rotated(Vec3::UnitY(), PI_F/2.0f));

    // Y
    auto yAxis = scene.createEntity("yAxis");
    yAxis.addComponent<LineRendererComponent>(Vec4(0.0f, 1.0f, 0.0f, 1.0f), 2.0f);
    setTransform(yAxis.getComponent<TransformComponent>(), Mat4(1.0f).rotated(-Vec3::UnitX(), PI_F/2.0f));

    // Y
    auto zAxis = scene.createEntity("zAxis");
    zAxis.addComponent<LineRendererComponent>(Vec4(0.0f, 0.0f, 1.0f, 1.0f), 2.0f);
    setTransform(zAxis.getComponent<TransformComponent>(), Mat4(1.0f));

    auto prevTime = std::chrono::high_resolution_clock::now();

    while(running) {
        const auto currTime = std::chrono::high_resolution_clock::now();
        const auto timeDiff = currTime - prevTime;
        prevTime = currTime;

        Timestep dt(std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count() / 1000.0f);

        window->update(); 
        scene.update(dt, window->getInput().get());
    }

    return 0; 
}