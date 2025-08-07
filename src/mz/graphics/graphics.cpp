module;
#include "mz/core/core.h"
module mz.graphics;

import mz.core.logging;
import mz.graphics.opengl;

namespace mz {

    std::shared_ptr<RenderBase> RenderBase::create(IGraphicsContext* context)
    {
#ifdef MZ_RENDERER_OPENGL
        return std::make_shared<GlRenderer>(context);
#endif

        MZ_ASSERT(false, "Failed to create Renderer");
    }

}

