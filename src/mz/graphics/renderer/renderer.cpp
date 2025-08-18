module;
#include "mz/core/core.h"
module mz.graphics.renderer;

import mz.core.logging;

import mz.graphics.renderer.opengl;
import mz.graphics.renderer.opengl.resources;

namespace mz {

    std::shared_ptr<IRenderer> RendererBase::create(IGraphicsContext* context)
    {
#ifdef MZ_RENDERER_OPENGL
        return std::make_shared<GlRenderer>(context);
#endif

        MZ_ASSERT(false, "Failed to create Renderer");
    }

    std::unique_ptr<IFrameBuffer> createFrameBuffer(const Vec2& size)
    {
#ifdef MZ_RENDERER_OPENGL
        return std::make_unique<GlFrameBuffer>(size);
#endif

        MZ_ASSERT(false, "Failed to create Framebuffer");
    }

}
