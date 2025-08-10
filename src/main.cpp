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


    // const auto pos = Vec3::createRandom(10.0f);

    // const auto view = Mat4::createLookAt(pos, Vec3(0.0f, 0.0f, 0.0f), Vec3::UnitY());
    // view.print();

    // auto viewTest = Mat4::Identity().translated(pos).inverted();
    // viewTest.print();

    // Test t;
    // printIterable<int>(&t);

    // auto arr = t.toArray();

    // ContainerWrapper<std::unique_ptr<float[]>> cont(new float[5]);
    // cont[2] = 5.0f;
    // auto v = cont.toVector();
    // auto 

    DynamicArray<int> arr(10, 3); 
    // printRange(arr);

    arr.resize(20, 4);
    // printRange(arr);

    arr.resize(4, 12);
    // printRange(arr);

    auto arr2 = arr.toArray<4>();
    // printRange(arr2);



    printIterable<int>(&arr2);
    

    running = false;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);

    return 0; 
}