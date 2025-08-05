import std;
import glm;

import mz.window;
import mz.window.glfw;

int main()
{
    std::unique_ptr<mz::IInput> input = std::make_unique<mz::glfw::Input>();

    glm::vec3 v(1.0f);
    std::println("Hello World! {}", glm::gtx::to_string(v));
}