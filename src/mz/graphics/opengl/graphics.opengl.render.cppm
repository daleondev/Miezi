module;
#include <glad/gl.h>
#include <cassert>
export module mz.graphics.opengl.render;

import std;
import glm;

import mz.graphics;
import mz.graphics.render;
import mz.graphics.opengl;
import mz.graphics.opengl.shader;

namespace mz {

    export class GlRenderer : public RenderBase
    {
    public:
        GlRenderer(IGraphicsContext* context) 
            : RenderBase(context, std::make_unique<GlShaderStore>()) 
        { 
            assert(context->is<GlGraphicsContext>() && "Invalid graphics context type");

            glEnable(GL_DEPTH_TEST);
            glClearDepthf(1.0f);
            glDepthFunc(GL_LESS);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_PROGRAM_POINT_SIZE);
            glEnable(GL_MULTISAMPLE);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        }
        ~GlRenderer() = default;

        void clear(const glm::vec4& color) override
        {
            m_context->makeCurrent();

            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };

}