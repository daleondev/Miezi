module;
#include "mz/core/core.h"
module mz.window;

import mz.core.logging;
import mz.window.glfw;

namespace mz {

    std::shared_ptr<IWindow> WindowBase::create(const std::string& title, const glm::vec2& size)
    {
#ifdef MZ_WINDOW_GLFW
        return std::make_shared<GlfwWindow>(title, size);
#endif

        MZ_ASSERT(false, "Failed to create Window");
    }

}

