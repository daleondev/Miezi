import std;
import glm;

import mz.window;
import mz.window.glfw;

int main()
{
    std::unique_ptr<mz::IWindow> window = std::make_unique<mz::GlfwWindow>("Test Window", glm::vec2{800, 600});

    while(true) {
        window->update();
    }

#ifdef MZ_OS_WINDOWS
    std::println("Hello Windows");
#elif defined(MZ_OS_LINUX)
    std::println("Hello Linux");
#endif
}