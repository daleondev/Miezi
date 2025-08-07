module;
#include "mz/core/core.h"
module mz.graphics.window;

import mz.core.logging;
import mz.graphics.window.glfw;

namespace mz {

    std::shared_ptr<WindowBase> WindowBase::create(const std::string& title, const glm::vec2& size)
    {
#ifdef MZ_WINDOW_GLFW
        return std::make_shared<GlfwWindow>(title, size);
#endif

        MZ_ASSERT(false, "Failed to create Window");
    }

}

