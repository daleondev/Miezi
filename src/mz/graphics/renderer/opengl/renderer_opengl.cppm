module;
#include "mz/core/core.h"

#include <glad/gl.h>
export module mz.graphics.renderer.opengl;

import std;

import mz.core.logging;

import mz.graphics.window;
import mz.graphics.renderer;
import mz.graphics.renderer.opengl.resources;

import mz.math.geometry;

namespace mz { 

    export class GlRenderContext : public IRenderContext
    {
    public:
        GlRenderContext(IWindow* window) : m_window{ window }, m_initialized{ false } {}
        virtual ~GlRenderContext() = default;

        void init(GLADloadfunc loadFunc)
        {
            if (m_initialized)
                return;

            const int version = gladLoadGL(loadFunc);
            MZ_ASSERT(version != 0, "Failed to initialize GLAD");
            MZ_INFO("OpenGL:");
            MZ_INFO("\tOpenGL Vendor: {}", (char*)glGetString(GL_VENDOR));
            MZ_INFO("\tOpenGL Renderer: {}", (char*)glGetString(GL_RENDERER));
            MZ_INFO("\tOpenGL Version: {}", (char*)glGetString(GL_VERSION));
            MZ_INFO("\tGLSL Version: {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
            MZ_INFO("\tGlad Version: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
            m_initialized = true;
        }

    protected:
        IWindow* m_window;

    private:
        bool m_initialized;

    };

    export class GlRenderer : public RenderBase
    {
    public:
        GlRenderer(IRenderContext* context) 
            : RenderBase(context, std::make_unique<GlShaderStore>()) 
        { 
            MZ_ASSERT(context->is<GlRenderContext>(), "Invalid graphics context type");

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

        void clear(const Vec4& color) override
        {
            m_context->makeCurrent();

            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };

}