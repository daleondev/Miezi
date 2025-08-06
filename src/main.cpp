import std;
import glm;

import mz.events;
import mz.events.window;

int main()
{
    std::unique_ptr<mz::IEvent> input = std::make_unique<mz::WindowResizeEvent>(30.0f, 40.0f);

    auto x = input->asPtr<mz::WindowResizeEvent>();
    if (x) {
        std::println("Width: {}, Height: {}, Handled: {}", x->getWidth(), x->getHeight(), x->isHandled());
        x->setHandled(true);
    }

    const mz::WindowResizeEvent& y = input->asRefUnchecked<mz::WindowResizeEvent>();
    std::println("Width: {}, Height: {}, Handled: {}", y.getWidth(), y.getHeight(), y.isHandled());
    

    glm::vec3 v(1.0f);
    std::println("Hello World! {}", glm::gtx::to_string(v));

#ifdef MZ_OS_WINDOWS
    std::println("Hello Windows");
#elif defined(MZ_OS_LINUX)
    std::println("Hello Linux");
#endif
}